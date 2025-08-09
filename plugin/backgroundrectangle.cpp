// SPDX-License-Identifier: LGPL-2.1-only

#include "backgroundrectangle.h"
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

BackgroundRectangle::BackgroundRectangle(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

void BackgroundRectangle::setColor(const QColor &c)
{
    if (m_color == c) return;
    m_color = c;
    emit colorChanged();
    update();
}

QSGNode *BackgroundRectangle::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode*>(oldNode);
    if (!node) {
        node = new QSGGeometryNode();
        auto *geom = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
        geom->setDrawingMode(GL_TRIANGLE_STRIP);
        node->setGeometry(geom);
        node->setFlag(QSGNode::OwnsGeometry, true);
        auto *mat = new QSGFlatColorMaterial();
        node->setMaterial(mat);
        node->setFlag(QSGNode::OwnsMaterial, true);
    }
    auto *geom = node->geometry();
    QRectF r = boundingRect();
    QSGGeometry::Point2D *v = geom->vertexDataAsPoint2D();
    v[0].set(r.left(),  r.top());
    v[1].set(r.right(), r.top());
    v[2].set(r.left(),  r.bottom());
    v[3].set(r.right(), r.bottom());
    static_cast<QSGFlatColorMaterial*>(node->material())->setColor(m_color);
    node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return node;
}
