// SPDX-License-Identifier: LGPL-2.1-only

#include "roundedwindowcorners.h"
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QQuickWindow>
#include <QtMath>

RoundedWindowCorners::RoundedWindowCorners(QQuickItem *parent)
    : QQuickItem(parent)
{
}

void RoundedWindowCorners::setRadius(qreal radius)
{
    if (!qFuzzyCompare(m_radius, radius)) {
        m_radius = radius;
        emit radiusChanged();
        update();
    }
}

void RoundedWindowCorners::setCorners(Corners corners)
{
    if (m_corners != corners) {
        m_corners = corners;
        emit cornersChanged();
        update();
    }
}

void RoundedWindowCorners::itemChange(ItemChange change, const ItemChangeData &value)
{
    if (change == ItemSceneChange) {
        updateWindowAssociation();
    }
    QQuickItem::itemChange(change, value);
}

QSGNode *RoundedWindowCorners::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);
    if (!node) {
        node = new QSGGeometryNode();
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
        material->setColor(Qt::black);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGGeometry *geometry = node->geometry();
    QSGFlatColorMaterial *material = static_cast<QSGFlatColorMaterial *>(node->material());

    // Create rounded corner geometry
    if (m_radius > 0.0) {
        const int segments = 8; // Number of segments for each corner
        const int totalVertices = segments * 4 * 3; // 4 corners, 3 vertices per triangle

        geometry->allocate(totalVertices);
        QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

        int vertexIndex = 0;
        qreal centerX, centerY;

        // Top-left corner
        if (m_corners & TopLeft) {
            centerX = m_radius;
            centerY = m_radius;
            for (int i = 0; i < segments; ++i) {
                qreal angle1 = M_PI + (i * M_PI / 2) / segments;
                qreal angle2 = M_PI + ((i + 1) * M_PI / 2) / segments;

                vertices[vertexIndex++].set(centerX, centerY);
                vertices[vertexIndex++].set(centerX + m_radius * qCos(angle1), centerY + m_radius * qSin(angle1));
                vertices[vertexIndex++].set(centerX + m_radius * qCos(angle2), centerY + m_radius * qSin(angle2));
            }
        }

        // Top-right corner
        if (m_corners & TopRight) {
            centerX = width() - m_radius;
            centerY = m_radius;
            for (int i = 0; i < segments; ++i) {
                qreal angle1 = -M_PI/2 + (i * M_PI / 2) / segments;
                qreal angle2 = -M_PI/2 + ((i + 1) * M_PI / 2) / segments;

                vertices[vertexIndex++].set(centerX, centerY);
                vertices[vertexIndex++].set(centerX + m_radius * qCos(angle1), centerY + m_radius * qSin(angle1));
                vertices[vertexIndex++].set(centerX + m_radius * qCos(angle2), centerY + m_radius * qSin(angle2));
            }
        }

        // Bottom-right corner
        if (m_corners & BottomRight) {
            centerX = width() - m_radius;
            centerY = height() - m_radius;
            for (int i = 0; i < segments; ++i) {
                qreal angle1 = (i * M_PI / 2) / segments;
                qreal angle2 = ((i + 1) * M_PI / 2) / segments;

                vertices[vertexIndex++].set(centerX, centerY);
                vertices[vertexIndex++].set(centerX + m_radius * qCos(angle1), centerY + m_radius * qSin(angle1));
                vertices[vertexIndex++].set(centerX + m_radius * qCos(angle2), centerY + m_radius * qSin(angle2));
            }
        }

        // Bottom-left corner
        if (m_corners & BottomLeft) {
            centerX = m_radius;
            centerY = height() - m_radius;
            for (int i = 0; i < segments; ++i) {
                qreal angle1 = M_PI/2 + (i * M_PI / 2) / segments;
                qreal angle2 = M_PI/2 + ((i + 1) * M_PI / 2) / segments;

                vertices[vertexIndex++].set(centerX, centerY);
                vertices[vertexIndex++].set(centerX + m_radius * qCos(angle1), centerY + m_radius * qSin(angle1));
                vertices[vertexIndex++].set(centerX + m_radius * qCos(angle2), centerY + m_radius * qSin(angle2));
            }
        }

        // Set the actual vertex count
        geometry->allocate(vertexIndex);
    } else {
        geometry->allocate(0);
    }

    geometry->markVertexDataDirty();
    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}

void RoundedWindowCorners::updateWindowAssociation()
{
    if (window()) {
        // TODO: Associate with window for proper clipping
        update();
    }
}
