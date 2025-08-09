// SPDX-License-Identifier: LGPL-2.1-only

#include "overlaygradient.h"
#include <QSGNode>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QQuickWindow>

OverlayGradient::OverlayGradient(QQuickItem *parent)
    : Silica::Item(parent)
{
}

void OverlayGradient::setStartColor(const QColor &color)
{
    if (m_startColor != color) {
        m_startColor = color;
        emit startColorChanged();
        update();
    }
}

void OverlayGradient::setEndColor(const QColor &color)
{
    if (m_endColor != color) {
        m_endColor = color;
        emit endColorChanged();
        update();
    }
}

void OverlayGradient::setNoise(const QUrl &noise)
{
    if (m_noise != noise) {
        m_noise = noise;
        emit noiseChanged();
        update();
    }
}

void OverlayGradient::setNoiseWeight(qreal weight)
{
    if (!qFuzzyCompare(m_noiseWeight, weight)) {
        m_noiseWeight = weight;
        emit noiseWeightChanged();
        update();
    }
}

void OverlayGradient::setDirection(Direction direction)
{
    if (m_direction != direction) {
        m_direction = direction;
        emit directionChanged();
        update();
    }
}

QSGNode *OverlayGradient::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);
    if (!node) {
        node = new QSGGeometryNode();
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
        geometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGGeometry *geometry = node->geometry();
    QSGFlatColorMaterial *material = static_cast<QSGFlatColorMaterial *>(node->material());

    // Calculate corner colors based on direction
    QColor topLeft = colorForCorner(0);
    QColor topRight = colorForCorner(1);
    QColor bottomRight = colorForCorner(2);
    QColor bottomLeft = colorForCorner(3);

    // Create gradient geometry
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    vertices[0].set(0, 0);
    vertices[1].set(width(), 0);
    vertices[2].set(0, height());
    vertices[3].set(width(), height());

    // Set material color (simplified - in a real implementation, you'd use a gradient shader)
    material->setColor(m_startColor);

    geometry->markVertexDataDirty();
    node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

    return node;
}

QColor OverlayGradient::colorForCorner(int corner) const
{
    // Simplified color calculation based on direction
    switch (m_direction) {
    case LeftToRight:
        return corner < 2 ? m_startColor : m_endColor;
    case RightToLeft:
        return corner < 2 ? m_endColor : m_startColor;
    case TopToBottom:
        return corner % 2 == 0 ? m_startColor : m_endColor;
    case BottomToTop:
        return corner % 2 == 0 ? m_endColor : m_startColor;
    default:
        return m_startColor;
    }
}

void OverlayGradient::updateGradient()
{
    update();
}
