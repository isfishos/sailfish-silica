// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEDIMMEDREGION_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEDIMMEDREGION_H

#include <QQuickItem>
#include <QColor>
#include <QRectF>
#include <QQmlListProperty>

class DeclarativeDimmedRegion : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QRectF area READ area WRITE setArea NOTIFY areaChanged)
    Q_PROPERTY(QQuickItem* target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(QQmlListProperty<QQuickItem> exclude READ excludeList)

public:
    explicit DeclarativeDimmedRegion(QQuickItem *parent = nullptr);

    QColor color() const { return m_color; }
    void setColor(const QColor &color);
    QRectF area() const { return m_area; }
    void setArea(const QRectF &area);
    QQuickItem* target() const { return m_target; }
    void setTarget(QQuickItem *target);
    QQmlListProperty<QQuickItem> excludeList();

    // QQmlListProperty helpers
    static void appendExclude(QQmlListProperty<QQuickItem> *prop, QQuickItem *item);
    static int excludeCount(QQmlListProperty<QQuickItem> *prop);
    static QQuickItem* excludeAt(QQmlListProperty<QQuickItem> *prop, int index);
    static void clearExclude(QQmlListProperty<QQuickItem> *prop);

Q_SIGNALS:
    void colorChanged();
    void areaChanged();
    void targetChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void updateRegion();
    void watchExcludedItems();
    void unwatchExcludedItems();

    QColor m_color = Qt::black;
    QRectF m_area;
    QQuickItem *m_target = nullptr;
    QList<QQuickItem*> m_excludedItems;
    QRegion m_computedRegion;
    bool m_regionDirty = true;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEDIMMEDREGION_H
