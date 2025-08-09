// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_LINEGRAPH_H
#define SAILFISH_SILICA_PLUGIN_LINEGRAPH_H

#include "lineitem.h"
#include <QVariantList>
#include <QVector2D>

class LineGraph : public LineItem
{
    Q_OBJECT
    Q_PROPERTY(QVariantList values READ values WRITE setValues NOTIFY valuesChanged)

public:
    explicit LineGraph(QQuickItem *parent = nullptr);

    QVariantList values() const { return m_values; }
    void setValues(const QVariantList &values);

Q_SIGNALS:
    void valuesChanged();

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void updateGraph();
    QVector<QVector2D> normalizeValues(const QVariantList &values) const;

    QVariantList m_values;
};

#endif // SAILFISH_SILICA_PLUGIN_LINEGRAPH_H
