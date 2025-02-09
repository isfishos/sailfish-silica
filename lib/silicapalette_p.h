#ifndef SILICAPALETTE_P_H
#define SILICAPALETTE_P_H

#include <silicapalette.h>
#include <silicatheme.h>
#include "themecolors.h"
#include <QSet>
#include <array>

namespace Silica {

class PalettePrivate
{
public:
    Q_DECLARE_PUBLIC(Palette)

    static PalettePrivate *get(Palette *palette) {
        return palette->d_func();
    }

    PalettePrivate();
    ~PalettePrivate();

    void setColorScheme(Theme::ColorScheme scheme, bool isExplicit);
    void updateColors();
    QColor derivativeColor(ColorIndex index) const;
    void setColor(ColorIndex index, const QColor &color, bool isExplicit);
    void resetColor(ColorIndex index);
    void updateParent(PalettePrivate* parent);

    Theme::ColorScheme m_colorScheme;
    QColor m_primaryColor;
    QColor m_secondaryColor;
    QColor m_highlightColor;
    QColor m_secondaryHighlightColor;
    QColor m_highlightBackgroundColor;
    QColor m_highlightDimmerColor;
    QColor m_overlayBackgroundColor;
    QColor m_backgroundGlowColor;
    QColor m_errorColor;
    QColor m_wallpaperOverlayColor;
    QColor m_coverOverlayColor;
    bool m_explicitColorScheme;
    QSet<PalettePrivate*> m_children;
    Palette *q_ptr;

private:
    using ColorChangedSignal = void (Palette::*)();
    void resetDerivativeColors();
    void emitColorChanged(ColorIndex index);
    QColor& colorFromIndex(ColorIndex index);

    std::array<bool, static_cast<std::size_t>(ColorIndex::ColorCount)> m_explicitColors;

    PalettePrivate* m_parent;
    ThemeColors* m_themeColors;
};

} // namespace Silica

#endif // SILICAPALETTE_P_H
