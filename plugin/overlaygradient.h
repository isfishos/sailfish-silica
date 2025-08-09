// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_OVERLAYGRADIENT_H
#define SAILFISH_SILICA_PLUGIN_OVERLAYGRADIENT_H

#include <silicaitem.h>
#include <QUrl>
#include <QColor>

class OverlayGradient : public Silica::Item
{
    Q_OBJECT
    Q_PROPERTY(QColor startColor READ startColor WRITE setStartColor NOTIFY startColorChanged)
    Q_PROPERTY(QColor endColor READ endColor WRITE setEndColor NOTIFY endColorChanged)
    Q_PROPERTY(QUrl noise READ noise WRITE setNoise NOTIFY noiseChanged)
    Q_PROPERTY(qreal noiseWeight READ noiseWeight WRITE setNoiseWeight NOTIFY noiseWeightChanged)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    enum Direction {
        LeftToRight = 0,
        RightToLeft = 1,
        TopToBottom = 2,
        BottomToTop = 3
    };
    Q_ENUM(Direction)

    explicit OverlayGradient(QQuickItem *parent = nullptr);

    QColor startColor() const { return m_startColor; }
    void setStartColor(const QColor &color);
    QColor endColor() const { return m_endColor; }
    void setEndColor(const QColor &color);
    QUrl noise() const { return m_noise; }
    void setNoise(const QUrl &noise);
    qreal noiseWeight() const { return m_noiseWeight; }
    void setNoiseWeight(qreal weight);
    Direction direction() const { return m_direction; }
    void setDirection(Direction direction);

Q_SIGNALS:
    void startColorChanged();
    void endColorChanged();
    void noiseChanged();
    void noiseWeightChanged();
    void directionChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private:
    void updateGradient();
    QColor colorForCorner(int corner) const;

    QColor m_startColor = Qt::transparent;
    QColor m_endColor = Qt::transparent;
    QUrl m_noise;
    qreal m_noiseWeight = 0.0;
    Direction m_direction = LeftToRight;
    bool m_noiseChanged = false;
    bool m_geometryChanged = false;
};

#endif // SAILFISH_SILICA_PLUGIN_OVERLAYGRADIENT_H
