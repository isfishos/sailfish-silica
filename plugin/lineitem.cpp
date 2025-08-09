// SPDX-License-Identifier: LGPL-2.1-only

#include "lineitem.h"
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QQuickWindow>

LineItem::LineItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

void LineItem::setCurveDensity(qreal density)
{
    if (m_curveDensity != density) {
        m_curveDensity = density;
        m_updatePoints = true;
        update();
        emit curveDensityChanged();
    }
}

void LineItem::setLineWidth(qreal width)
{
    if (m_lineWidth != width) {
        m_lineWidth = width;
        m_updatePoints = true;
        update();
        emit lineWidthChanged();
    }
}

void LineItem::setColor(const QColor &color)
{
    if (m_color != color) {
        m_color = color;
        update();
        emit colorChanged();
    }
}

void LineItem::setPoints(int series, const QVector<QVector2D> &points)
{
    if (series >= m_series.size()) {
        m_series.resize(series + 1);
    }

    m_series[series] = points;
    m_updatePoints = true;
    update();
}

QSGNode *LineItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);

    if (!node) {
        node = new QSGGeometryNode();
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
        material->setColor(m_color);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        QSGFlatColorMaterial *material = static_cast<QSGFlatColorMaterial *>(node->material());
        material->setColor(m_color);
    }

    if (m_updatePoints) {
        updateGeometry(node);
        m_updatePoints = false;
    }

    return node;
}

void LineItem::updateGeometry(QSGGeometryNode *node)
{
    if (m_series.isEmpty()) {
        return;
    }

    // Calculate total number of vertices needed
    int totalVertices = 0;
    for (const QVector<QVector2D> &series : m_series) {
        if (series.size() > 1) {
            totalVertices += series.size();
        }
    }

    if (totalVertices == 0) {
        return;
    }

    // Create geometry for line strips
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), totalVertices);
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    int vertexIndex = 0;
    for (const QVector<QVector2D> &series : m_series) {
        if (series.size() > 1) {
            QVector<QVector2D> smoothedPoints = smoothCurve(series);

            for (const QVector2D &point : smoothedPoints) {
                vertices[vertexIndex].set(point.x(), point.y());
                vertexIndex++;
            }
        }
    }

    geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
    geometry->setLineWidth(m_lineWidth);

    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);
}

QVector<QVector2D> LineItem::smoothCurve(const QVector<QVector2D> &points) const
{
    if (points.size() < 2) {
        return points;
    }

    if (m_curveDensity <= 1.0) {
        return points;
    }

    // Simple smoothing by adding intermediate points
    QVector<QVector2D> smoothed;
    smoothed.reserve(points.size() * m_curveDensity);

    for (int i = 0; i < points.size() - 1; ++i) {
        const QVector2D &p1 = points[i];
        const QVector2D &p2 = points[i + 1];

        smoothed.append(p1);

        // Add intermediate points based on curve density
        int intermediatePoints = qMax(1, static_cast<int>(m_curveDensity - 1));
        for (int j = 1; j <= intermediatePoints; ++j) {
            qreal t = j / qreal(intermediatePoints + 1);
            QVector2D intermediate = p1 + t * (p2 - p1);
            smoothed.append(intermediate);
        }
    }

    smoothed.append(points.last());
    return smoothed;
}
