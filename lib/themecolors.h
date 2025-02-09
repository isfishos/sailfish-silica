#ifndef THEMECOLORS_H
#define THEMECOLORS_H

#include <QColor>
#include <silicatheme.h>
#include <silicatheme_p.h>
#include <mdconfgroup.h>
#include <QSet>

namespace Silica {

class PalettePrivate;
class ThemePrivate;

class ThemeColors : public MDConfGroup {
    Q_OBJECT
    Q_PROPERTY(QString primary WRITE setPrimary)
    Q_PROPERTY(QString secondary WRITE setSecondary)
    Q_PROPERTY(QString highlight WRITE setHighlight)
    Q_PROPERTY(QString secondaryHighlight WRITE setSecondaryHighlight)
    Q_PROPERTY(QString highlightBackground WRITE setHighlightBackground)
    Q_PROPERTY(QString highlightDimmer WRITE setHighlightDimmer)
    Q_PROPERTY(QString overlayBackground WRITE setOverlayBackground)
    Q_PROPERTY(QString backgroundGlow WRITE setBackgroundGlow)
    Q_PROPERTY(QString error WRITE setError)

public:
    explicit ThemeColors(ThemePrivate *theme);
    using MDConfGroup::resolveMetaObject;

    QColor color(ColorIndex index) const;
    void setColor(ColorIndex index, const QColor &color);

    void setPrimary(const QString &value) { setColor(ColorIndex::Primary, QColor(value)); }
    void setSecondary(const QString &value) { setColor(ColorIndex::Secondary, QColor(value)); }
    void setHighlight(const QString &value) { setColor(ColorIndex::Highlight, QColor(value)); }
    void setSecondaryHighlight(const QString &value) { setColor(ColorIndex::SecondaryHighlight, QColor(value)); }
    void setHighlightBackground(const QString &value) { setColor(ColorIndex::HighlightBackground, QColor(value)); }
    void setHighlightDimmer(const QString &value) { setColor(ColorIndex::HighlightDimmer, QColor(value)); }
    void setOverlayBackground(const QString &value) { setColor(ColorIndex::OverlayBackground, QColor(value)); }
    void setBackgroundGlow(const QString &value) { setColor(ColorIndex::BackgroundGlow, QColor(value)); }
    void setError(const QString &value) { setColor(ColorIndex::Error, QColor(value)); }

    void addPalette(PalettePrivate* palette);
    const QSet<PalettePrivate*>& palettes() const { return m_palettes; }

private:
    ThemePrivate *m_theme;
    QSet<PalettePrivate*> m_palettes;
};

} // namespace Silica

#endif // THEMECOLORS_H
