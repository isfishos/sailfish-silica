#include "themecolors.h"
#include "silicapalette_p.h"
#include "silicatheme_p.h"
#include "silicatheme.h"

namespace Silica {

ThemeColors::ThemeColors(ThemePrivate *theme)
    : MDConfGroup("/desktop/jolla/theme/color")
    , m_theme(theme)
{
}

QColor ThemeColors::color(ColorIndex index) const
{
    // Check for runtime override first
    auto it = m_runtimeColors.find(index);
    if (it != m_runtimeColors.end()) {
        return it.value();
    }

    // Fall back to computed defaults
    return computeDefaultColor(index);
}

void ThemeColors::setColor(ColorIndex index, const QColor &color)
{
    if (color.isValid()) {
        m_runtimeColors[index] = color;
    } else {
        m_runtimeColors.remove(index);
    }

    // Notify palettes of color change
    refreshPalettes();
}

QColor ThemeColors::computeDefaultColor(ColorIndex index) const
{
    Theme::ColorScheme currentScheme = m_theme->m_colorScheme;
    QColor baseHighlight = getBaseHighlightColor();

    return getColorForScheme(index, currentScheme, baseHighlight);
}

QColor ThemeColors::getBaseHighlightColor() const
{
    // Standard default highlight color
    return QColor(127, 223, 255);
}

QColor ThemeColors::getColorForScheme(ColorIndex index, Theme::ColorScheme scheme, const QColor &highlight)
{
    using CS = Theme::ColorScheme;

    switch (index) {
    case ColorIndex::Primary:
        return (scheme == CS::DarkOnLight)
            ? QColor(0, 0, 0)
            : QColor(255, 255, 255);

    case ColorIndex::Secondary:
        return (scheme == CS::DarkOnLight)
            ? QColor(69, 69, 69)
            : QColor(186, 186, 186);

    case ColorIndex::Highlight:
        return Theme::highlightFromColor(highlight, scheme);

    case ColorIndex::SecondaryHighlight:
        return Theme::secondaryHighlightFromColor(highlight, scheme);

    case ColorIndex::HighlightBackground:
        return Theme::highlightBackgroundFromColor(highlight, scheme);

    case ColorIndex::HighlightDimmer:
        return Theme::highlightDimmerFromColor(highlight, scheme);

    case ColorIndex::OverlayBackground:
        return (scheme == CS::DarkOnLight)
            ? QColor(0xEE, 0xEE, 0xEE)
            : QColor(Qt::black);

    case ColorIndex::BackgroundGlow:
        return (scheme == CS::DarkOnLight) ? highlight : QColor(Qt::transparent);

    case ColorIndex::Error:
        return (scheme == CS::DarkOnLight) ? QColor("#f62929") : QColor("#ff4d4d");

    case ColorIndex::WallpaperOverlay:
        return (scheme == CS::DarkOnLight)
            ? QColor(255, 255, 255, 0x5A)
            : QColor(0, 0, 0, 0x99);

    case ColorIndex::CoverOverlay:
        if (scheme == CS::DarkOnLight) {
            return QColor::fromHsvF(highlight.hueF(), 0.5, 1.0, 0.3).toRgb();
        } else {
            return QColor::fromHsvF(highlight.hueF(), 1.0, 0.4, 0.5).toRgb();
        }

    default:
        return QColor();
    }
}

void ThemeColors::refreshPalettes()
{
    for (PalettePrivate *palette : m_palettes) {
        if (palette) {
            palette->updateColors();
        }
    }
}

void ThemeColors::addPalette(PalettePrivate* palette)
{
    if (palette && !m_palettes.contains(palette)) {
        m_palettes.insert(palette);
        palette->updateColors();
    }
}

void ThemeColors::removePalette(PalettePrivate* palette)
{
    if (palette) {
        m_palettes.remove(palette);
    }
}

} // namespace Silica