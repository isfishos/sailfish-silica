#include "silicapalette.h"
#include "silicapalette_p.h"
#include "silicatheme_p.h"

namespace Silica {

PalettePrivate::PalettePrivate()
    : m_colorScheme(Theme::LightOnDark)
    , m_explicitColorScheme(false)
    , q_ptr(nullptr)
    , m_parent(nullptr)
    , m_themeColors(ThemePrivate::instance()->themeColors())
    , m_explicitColors{}
{
}

QColor& PalettePrivate::colorFromIndex(ColorIndex index)
{
    switch (index) {
    case ColorIndex::Primary: return m_primaryColor;
    case ColorIndex::Secondary: return m_secondaryColor;
    case ColorIndex::Highlight: return m_highlightColor;
    case ColorIndex::SecondaryHighlight: return m_secondaryHighlightColor;
    case ColorIndex::HighlightBackground: return m_highlightBackgroundColor;
    case ColorIndex::HighlightDimmer: return m_highlightDimmerColor;
    case ColorIndex::OverlayBackground: return m_overlayBackgroundColor;
    case ColorIndex::BackgroundGlow: return m_backgroundGlowColor;
    case ColorIndex::Error: return m_errorColor;
    case ColorIndex::WallpaperOverlay: return m_wallpaperOverlayColor;
    case ColorIndex::CoverOverlay: return m_coverOverlayColor;
    default:
        Q_ASSERT_X(false, "PalettePrivate::colorFromIndex", "Invalid color index");
        return m_primaryColor;
    }
}

void PalettePrivate::setColorScheme(Theme::ColorScheme scheme, bool isExplicit)
{
    m_explicitColorScheme = isExplicit;

    if (m_colorScheme != scheme) {
        m_colorScheme = scheme;

        if (q_ptr) {
            emit q_ptr->colorSchemeChanged();
        }

        // Propagate to child palettes that don't have explicit color scheme
        for (PalettePrivate *child : m_children) {
            if (!child->m_explicitColorScheme) {
                child->setColorScheme(scheme, false);
            }
        }

        updateColors();
    }
}

void PalettePrivate::emitColorChanged(ColorIndex index)
{
    if (!q_ptr) {
        return;
    }

    switch (index) {
    case ColorIndex::Primary:
        emit q_ptr->primaryColorChanged();
        break;
    case ColorIndex::Secondary:
        emit q_ptr->secondaryColorChanged();
        break;
    case ColorIndex::Highlight:
        emit q_ptr->highlightColorChanged();
        break;
    case ColorIndex::SecondaryHighlight:
        emit q_ptr->secondaryHighlightColorChanged();
        break;
    case ColorIndex::HighlightBackground:
        emit q_ptr->highlightBackgroundColorChanged();
        break;
    case ColorIndex::HighlightDimmer:
        emit q_ptr->highlightDimmerColorChanged();
        break;
    case ColorIndex::OverlayBackground:
        emit q_ptr->overlayBackgroundColorChanged();
        break;
    case ColorIndex::BackgroundGlow:
        emit q_ptr->backgroundGlowColorChanged();
        break;
    case ColorIndex::Error:
        emit q_ptr->errorColorChanged();
        break;
    case ColorIndex::WallpaperOverlay:
        emit q_ptr->wallpaperOverlayColorChanged();
        break;
    case ColorIndex::CoverOverlay:
        emit q_ptr->coverOverlayColorChanged();
        break;
    default:
        Q_ASSERT_X(false, "PalettePrivate::emitColorChanged", "Invalid color index");
    }
}

void PalettePrivate::setColor(ColorIndex index, const QColor &color, bool isExplicit)
{
    m_explicitColors[static_cast<size_t>(index)] = isExplicit;
    QColor &currentColor = colorFromIndex(index);

    if (currentColor != color) {
        currentColor = color;
        emitColorChanged(index);

        // Handle derivative colors when highlight color changes
        if (index == ColorIndex::Highlight && !color.isValid()) {
            resetDerivativeColors();
        }

        // Propagate to child palettes that don't have explicit colors
        for (PalettePrivate *child : m_children) {
            if (!child->m_explicitColors[static_cast<size_t>(index)]) {
                child->resetColor(index);
            }
        }
    }
}

void PalettePrivate::resetDerivativeColors()
{
    static const ColorIndex derivatives[] = {
        ColorIndex::SecondaryHighlight,
        ColorIndex::HighlightBackground,
        ColorIndex::HighlightDimmer,
        ColorIndex::BackgroundGlow,
        ColorIndex::CoverOverlay
    };

    for (ColorIndex index : derivatives) {
        if (!m_explicitColors[static_cast<size_t>(index)]) {
            resetColor(index);
        }
    }
}

void PalettePrivate::resetColor(ColorIndex index)
{
    setColor(index, derivativeColor(index), false);
}

QColor PalettePrivate::derivativeColor(ColorIndex index) const
{
    if (m_parent) {
        return m_parent->derivativeColor(index);
    }

    // We map directly to the same ColorIndex enum
    return m_themeColors->color(index);
}

void PalettePrivate::updateParent(PalettePrivate* parent)
{
    if (m_parent == parent)
        return;

    if (m_parent)
        m_parent->m_children.remove(this);

    m_parent = parent;

    if (m_parent)
        m_parent->m_children.insert(this);
    else if (m_themeColors)
        m_themeColors->addPalette(this);

    updateColors();
}

PalettePrivate::~PalettePrivate()
{
}

void PalettePrivate::updateColors()
{
    // For each color index, if not explicitly set, derive value
    const ColorIndex allIndices[] = {
        ColorIndex::Primary,
        ColorIndex::Secondary,
        ColorIndex::Highlight,
        ColorIndex::SecondaryHighlight,
        ColorIndex::HighlightBackground,
        ColorIndex::HighlightDimmer,
        ColorIndex::OverlayBackground,
        ColorIndex::BackgroundGlow,
        ColorIndex::Error,
        ColorIndex::WallpaperOverlay,
        ColorIndex::CoverOverlay
    };

    for (ColorIndex idx : allIndices) {
        if (!m_explicitColors[static_cast<size_t>(idx)]) {
            QColor value = derivativeColor(idx);
            QColor &ref = colorFromIndex(idx);
            if (ref != value) {
                ref = value;
                emitColorChanged(idx);
            }
        }
    }
}

Palette::Palette(QObject *parent)
    : QObject(parent)
    , d_ptr(new PalettePrivate)
{
    d_ptr->q_ptr = this;  // Set back-pointer to public class
}

Palette::~Palette()
{
}

Theme::ColorScheme Palette::colorScheme() const
{
    return d_ptr->m_colorScheme;
}

void Palette::setColorScheme(Theme::ColorScheme scheme)
{
    d_ptr->setColorScheme(scheme, true);
}

void Palette::resetColorScheme()
{
    setColorScheme(Theme::LightOnDark);
}

QColor Palette::primaryColor() const
{
    return d_ptr->m_primaryColor;
}

void Palette::setPrimaryColor(const QColor &color)
{
    d_ptr->setColor(ColorIndex::Primary, color, true);
}

void Palette::resetPrimaryColor()
{
    setPrimaryColor(QColor());
}

QColor Palette::secondaryColor() const
{
    return d_ptr->m_secondaryColor;
}

void Palette::setSecondaryColor(const QColor &color)
{
    d_ptr->setColor(ColorIndex::Secondary, color, true);
}

void Palette::resetSecondaryColor() const
{
    const_cast<Palette*>(this)->setSecondaryColor(QColor());
}

QColor Palette::highlightColor() const
{
    return d_ptr->m_highlightColor;
}

void Palette::setHighlightColor(const QColor &color)
{
    d_ptr->setColor(ColorIndex::Highlight, color, true);
}

void Palette::resetHighlightColor()
{
    setHighlightColor(QColor());
}

QColor Palette::secondaryHighlightColor() const
{
    return d_ptr->m_secondaryHighlightColor;
}

void Palette::setSecondaryHighlightColor(const QColor &color)
{
    d_ptr->setColor(ColorIndex::SecondaryHighlight, color, true);
}

void Palette::resetSecondaryHighlightColor()
{
    setSecondaryHighlightColor(QColor());
}

QColor Palette::highlightBackgroundColor()
{
    return d_ptr->m_highlightBackgroundColor;
}

void Palette::setHighlightBackgroundColor(const QColor &color)
{
    d_ptr->setColor(ColorIndex::HighlightBackground, color, true);
}

void Palette::resetHighlightBackgroundColor()
{
    setHighlightBackgroundColor(QColor());
}

QColor Palette::highlightDimmerColor()
{
    return d_ptr->m_highlightDimmerColor;
}

void Palette::setHighlightDimmerColor(const QColor &color)
{
    d_ptr->setColor(ColorIndex::HighlightDimmer, color, true);
}

void Palette::resetHighlightDimmerColor()
{
    setHighlightDimmerColor(QColor());
}

QColor Palette::overlayBackgroundColor() const
{
    return d_ptr->m_overlayBackgroundColor;
}

void Palette::setOverlayBackgroundColor(const QColor &color)
{
    d_ptr->setColor(ColorIndex::OverlayBackground, color, true);
}

void Palette::resetOverlayBackgroundColor()
{
    setOverlayBackgroundColor(QColor());
}

QColor Palette::backgroundGlowColor() const
{
    return d_ptr->m_backgroundGlowColor;
}

void Palette::setBackgroundGlowColor(const QColor &color)
{
    d_ptr->setColor(ColorIndex::BackgroundGlow, color, true);
}

void Palette::resetBackgroundGlowColor()
{
    setBackgroundGlowColor(QColor());
}

QColor Palette::errorColor() const
{
    return d_ptr->m_errorColor;
}

void Palette::setErrorColor(const QColor &color)
{
    d_ptr->setColor(ColorIndex::Error, color, true);
}

void Palette::resetErrorColor()
{
    setErrorColor(QColor());
}

QColor Palette::wallpaperOverlayColor() const
{
    return d_ptr->m_wallpaperOverlayColor;
}

QColor Palette::coverOverlayColor() const
{
    return d_ptr->m_coverOverlayColor;
}

} // namespace Silica
