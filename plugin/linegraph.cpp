// SPDX-License-Identifier: LGPL-2.1-only

#include "linegraph.h"
#include <QVariant>
#include <QVector2D>

LineGraph::LineGraph(QQuickItem *parent)
    : LineItem(parent)
{
}

void LineGraph::setValues(const QVariantList &values)
{
    if (m_values != values) {
        m_values = values;
        updateGraph();
        emit valuesChanged();
    }
}

void LineGraph::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    LineItem::geometryChanged(newGeometry, oldGeometry);
    updateGraph();
}

void LineGraph::updateGraph()
{
    if (m_values.isEmpty()) {
        setPoints(0, QVector<QVector2D>());
        return;
    }

    QVector<QVector2D> normalizedPoints = normalizeValues(m_values);
    setPoints(0, normalizedPoints);
}

QVector<QVector2D> LineGraph::normalizeValues(const QVariantList &values) const
{
    if (values.isEmpty()) {
        return QVector<QVector2D>();
    }

    // Find min and max values
    qreal minVal = std::numeric_limits<qreal>::max();
    qreal maxVal = std::numeric_limits<qreal>::lowest();

    for (const QVariant &value : values) {
        bool ok;
        qreal numValue = value.toReal(&ok);
        if (ok) {
            minVal = qMin(minVal, numValue);
            maxVal = qMax(maxVal, numValue);
        }
    }

    // Handle flat lines (min == max)
    if (qFuzzyCompare(minVal, maxVal)) {
        minVal = 0.0;
        maxVal = 1.0;
    }

    // Normalize values to [0,1] range and map to item coordinates
    QVector<QVector2D> points;
    points.reserve(values.size());

    qreal width = boundingRect().width();
    qreal height = boundingRect().height();
    qreal padding = 10.0; // Padding from edges

    for (int i = 0; i < values.size(); ++i) {
        const QVariant &value = values[i];
        bool ok;
        qreal numValue = value.toReal(&ok);

        if (ok) {
            // Normalize to [0,1]
            qreal normalized = (numValue - minVal) / (maxVal - minVal);

            // Map to item coordinates with padding
            qreal x = padding + (width - 2 * padding) * (i / qreal(values.size() - 1));
            qreal y = height - padding - (height - 2 * padding) * normalized;

            points.append(QVector2D(x, y));
        }
    }

    return points;
}
