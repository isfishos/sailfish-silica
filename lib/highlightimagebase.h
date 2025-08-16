#ifndef HIGHLIGHTIMAGEBASE_H
#define HIGHLIGHTIMAGEBASE_H

#include <QColor>
#include <QImage>
#include <private/qquickimage_p.h>

namespace Silica {

class Palette;

class HighlightImageBase : public QQuickImage
{
    Q_OBJECT
    Q_PROPERTY(Silica::Palette *palette READ palette CONSTANT)
    Q_PROPERTY(bool highlighted READ highlighted WRITE setHighlighted NOTIFY highlightedChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)
    Q_PROPERTY(double colorWeight READ colorWeight WRITE setColorWeight NOTIFY colorWeightChanged)
    Q_PROPERTY(double monochromeWeight READ monochromeWeight WRITE setMonochromeWeight NOTIFY monochromeWeightChanged)


public:
    explicit HighlightImageBase(QQuickItem *parent = nullptr);

    Silica::Palette *palette() const;
    bool highlighted() const;
    QColor color() const;
    QColor highlightColor() const;
    double colorWeight() const;
    double monochromeWeight() const;

    void setHighlighted(bool highlighted);
    void setColor(const QColor &color);
    void setHighlightColor(const QColor &highlightColor);
    void setColorWeight(double weight);
    void setMonochromeWeight(double weight);

signals:
    void highlightedChanged();
    void colorChanged();
    void highlightColorChanged();
    void colorWeightChanged();
    void monochromeWeightChanged();

private:
    bool m_highlighted;
    QColor m_color;
    QColor m_highlightColor;
    double m_colorWeight;
    double m_monochromeWeight;
    Silica::Palette *m_palette;
};

} // namespace Silica

#endif // HIGHLIGHTIMAGEBASE_H
