// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_LINEITEM_H
#define SAILFISH_SILICA_PLUGIN_LINEITEM_H

#include <QQuickItem>
#include <QSGNode>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QVector>
#include <QVector2D>
#include <QColor>

class LineItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal curveDensity READ curveDensity WRITE setCurveDensity NOTIFY curveDensityChanged)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit LineItem(QQuickItem *parent = nullptr);

    qreal curveDensity() const { return m_curveDensity; }
    void setCurveDensity(qreal density);
    qreal lineWidth() const { return m_lineWidth; }
    void setLineWidth(qreal width);
    QColor color() const { return m_color; }
    void setColor(const QColor &color);

    void setPoints(int series, const QVector<QVector2D> &points);

Q_SIGNALS:
    void curveDensityChanged();
    void lineWidthChanged();
    void colorChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    void updateGeometry(QSGGeometryNode *node);
    QVector<QVector2D> smoothCurve(const QVector<QVector2D> &points) const;

    qreal m_curveDensity = 1.0;
    qreal m_lineWidth = 1.0;
    QColor m_color = Qt::black;
    QVector<QVector<QVector2D>> m_series;
    bool m_updatePoints = true;
};

#endif // SAILFISH_SILICA_PLUGIN_LINEITEM_H
