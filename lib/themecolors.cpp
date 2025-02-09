#include "themecolors.h"
#include "silicapalette_p.h"

namespace Silica {

ThemeColors::ThemeColors(ThemePrivate *theme)
    : MDConfGroup("/desktop/jolla/theme/color")
    , m_theme(theme)
{
}

QColor ThemeColors::color(ColorIndex index) const
{
    switch (index) {
    case ColorIndex::Primary: return property("primary").toString();
    case ColorIndex::Secondary: return property("secondary").toString();
    case ColorIndex::Highlight: return property("highlight").toString();
    case ColorIndex::SecondaryHighlight: return property("secondaryHighlight").toString();
    case ColorIndex::HighlightBackground: return property("highlightBackground").toString();
    case ColorIndex::HighlightDimmer: return property("highlightDimmer").toString();
    case ColorIndex::OverlayBackground: return property("overlayBackground").toString();
    case ColorIndex::BackgroundGlow: return property("backgroundGlow").toString();
    case ColorIndex::Error: return property("error").toString();
    case ColorIndex::WallpaperOverlay: return property("wallpaperOverlay").toString();
    case ColorIndex::CoverOverlay: return property("coverOverlay").toString();
    default:
        Q_ASSERT_X(false, "ThemeColors::color", "Invalid color index");
        return QColor();
    }
}

void ThemeColors::setColor(ColorIndex index, const QColor &color)
{
    switch (index) {
    case ColorIndex::Primary: setProperty("primary", color.name()); break;
    case ColorIndex::Secondary: setProperty("secondary", color.name()); break;
    case ColorIndex::Highlight: setProperty("highlight", color.name()); break;
    case ColorIndex::SecondaryHighlight: setProperty("secondaryHighlight", color.name()); break;
    case ColorIndex::HighlightBackground: setProperty("highlightBackground", color.name()); break;
    case ColorIndex::HighlightDimmer: setProperty("highlightDimmer", color.name()); break;
    case ColorIndex::OverlayBackground: setProperty("overlayBackground", color.name()); break;
    case ColorIndex::BackgroundGlow: setProperty("backgroundGlow", color.name()); break;
    case ColorIndex::Error: setProperty("error", color.name()); break;
    case ColorIndex::WallpaperOverlay: setProperty("wallpaperOverlay", color.name()); break;
    case ColorIndex::CoverOverlay: setProperty("coverOverlay", color.name()); break;
    default:
        Q_ASSERT_X(false, "ThemeColors::setColor", "Invalid color index");
    }
}

void ThemeColors::addPalette(PalettePrivate* palette)
{
    if (palette && !m_palettes.contains(palette)) {
        m_palettes.insert(palette);
        palette->updateColors();
    }
}

} // namespace Silica
