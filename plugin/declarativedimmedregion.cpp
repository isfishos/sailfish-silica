// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativedimmedregion.h"
#include <QSGNode>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QRegion>
#include <QPainterPath>

DeclarativeDimmedRegion::DeclarativeDimmedRegion(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

void DeclarativeDimmedRegion::setColor(const QColor &color)
{
    if (m_color != color) {
        m_color = color;
        update();
        emit colorChanged();
    }
}

void DeclarativeDimmedRegion::setArea(const QRectF &area)
{
    if (m_area != area) {
        m_area = area;
        m_regionDirty = true;
        update();
        emit areaChanged();
    }
}

void DeclarativeDimmedRegion::setTarget(QQuickItem *target)
{
    if (m_target != target) {
        m_target = target;
        m_regionDirty = true;
        update();
        emit targetChanged();
    }
}

QQmlListProperty<QQuickItem> DeclarativeDimmedRegion::excludeList()
{
    return QQmlListProperty<QQuickItem>(this, nullptr, appendExclude, excludeCount, excludeAt, clearExclude);
}

void DeclarativeDimmedRegion::appendExclude(QQmlListProperty<QQuickItem> *prop, QQuickItem *item)
{
    DeclarativeDimmedRegion *region = qobject_cast<DeclarativeDimmedRegion*>(prop->object);
    if (region && item) {
        region->m_excludedItems.append(item);
        region->m_regionDirty = true;
        region->watchExcludedItems();
        region->update();
    }
}

int DeclarativeDimmedRegion::excludeCount(QQmlListProperty<QQuickItem> *prop)
{
    DeclarativeDimmedRegion *region = qobject_cast<DeclarativeDimmedRegion*>(prop->object);
    return region ? region->m_excludedItems.count() : 0;
}

QQuickItem* DeclarativeDimmedRegion::excludeAt(QQmlListProperty<QQuickItem> *prop, int index)
{
    DeclarativeDimmedRegion *region = qobject_cast<DeclarativeDimmedRegion*>(prop->object);
    return region && index >= 0 && index < region->m_excludedItems.count() 
           ? region->m_excludedItems.at(index) : nullptr;
}

void DeclarativeDimmedRegion::clearExclude(QQmlListProperty<QQuickItem> *prop)
{
    DeclarativeDimmedRegion *region = qobject_cast<DeclarativeDimmedRegion*>(prop->object);
    if (region) {
        region->unwatchExcludedItems();
        region->m_excludedItems.clear();
        region->m_regionDirty = true;
        region->update();
    }
}

QSGNode *DeclarativeDimmedRegion::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (m_regionDirty) {
        updateRegion();
    }

    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);
    if (!node) {
        node = new QSGGeometryNode;
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(m_color);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        QSGFlatColorMaterial *material = static_cast<QSGFlatColorMaterial *>(node->material());
        if (material->color() != m_color) {
            material->setColor(m_color);
        }
    }

    // Convert QRegion to triangles for rendering
    QPainterPath path;
    if (m_computedRegion.isEmpty()) {
        // Fill entire item if no region computed
        path.addRect(boundingRect());
    } else {
        // Convert region to path
        path.addRegion(m_computedRegion);
    }

    QSGGeometry *geometry = node->geometry();
    QVector<QPointF> points = path.toFillPolygon();

    // Convert to triangles (simple triangulation)
    QVector<QPointF> triangles;
    for (int i = 1; i < points.size() - 1; ++i) {
        triangles.append(points[0]);
        triangles.append(points[i]);
        triangles.append(points[i + 1]);
    }

    geometry->allocate(triangles.size());
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    for (int i = 0; i < triangles.size(); ++i) {
        vertices[i].set(triangles[i].x(), triangles[i].y());
    }

    return node;
}

void DeclarativeDimmedRegion::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    if (newGeometry != oldGeometry) {
        m_regionDirty = true;
        update();
    }
}

void DeclarativeDimmedRegion::updateRegion()
{
    m_regionDirty = false;

    // Start with item bounds or specified area
    QRectF baseRect = m_area.isValid() ? m_area : boundingRect();
    QRegion region = QRegion(baseRect.toRect());

    // Subtract excluded items
    for (QQuickItem *item : m_excludedItems) {
        if (item && item->isVisible()) {
            QRectF itemRect = item->mapRectToItem(this, item->boundingRect());
            region = region.subtracted(QRegion(itemRect.toRect()));
        }
    }

    m_computedRegion = region;
}

void DeclarativeDimmedRegion::watchExcludedItems()
{
    // Connect to geometry/visibility changes of excluded items
    for (QQuickItem *item : m_excludedItems) {
        if (item) {
            connect(item, &QQuickItem::widthChanged, this, [this]() {
                m_regionDirty = true;
                update();
            });
            connect(item, &QQuickItem::heightChanged, this, [this]() {
                m_regionDirty = true;
                update();
            });
            connect(item, &QQuickItem::visibleChanged, this, [this]() {
                m_regionDirty = true;
                update();
            });
        }
    }
}

void DeclarativeDimmedRegion::unwatchExcludedItems()
{
    // Disconnect from all excluded items
    for (QQuickItem *item : m_excludedItems) {
        if (item) {
            disconnect(item, nullptr, this, nullptr);
        }
    }
}
