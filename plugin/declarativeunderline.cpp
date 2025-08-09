// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativeunderline.h"
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QQuickWindow>

DeclarativeUnderline::DeclarativeUnderline(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

void DeclarativeUnderline::setHorizontalAlignment(int alignment)
{
    if (m_horizontalAlignment != alignment) {
        m_horizontalAlignment = alignment;
        m_geometryDirty = true;
        update();
        emit horizontalAlignmentChanged();
    }
}

void DeclarativeUnderline::setPrimaryColor(const QColor &color)
{
    if (m_primaryColor != color) {
        m_primaryColor = color;
        m_geometryDirty = true;
        update();
        emit primaryColorChanged();
    }
}

void DeclarativeUnderline::setSecondaryColor(const QColor &color)
{
    if (m_secondaryColor != color) {
        m_secondaryColor = color;
        m_geometryDirty = true;
        update();
        emit secondaryColorChanged();
    }
}

QSGNode *DeclarativeUnderline::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);

    if (!node) {
        node = new QSGGeometryNode();
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    if (m_geometryDirty) {
        updateGeometry(node);
        m_geometryDirty = false;
    }

    return node;
}

void DeclarativeUnderline::updateGeometry(QSGGeometryNode *node)
{
    const qreal width = boundingRect().width();
    const qreal height = boundingRect().height();

    if (width <= 0 || height <= 0) {
        return;
    }

    // Create a thin underline band near the bottom
    const qreal underlineHeight = 2.0;
    const qreal y = height - underlineHeight;

    // Create gradient vertices based on alignment
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
    QSGGeometry::TexturedPoint2D *vertices = geometry->vertexDataAsTexturedPoint2D();

    // Create a gradient effect by varying color along the width
    const int segments = 20; // Number of color segments for smooth gradient
    QVector<QSGGeometry::TexturedPoint2D> allVertices;

    for (int i = 0; i < segments; ++i) {
        qreal x1 = (i * width) / segments;
        qreal x2 = ((i + 1) * width) / segments;
        qreal pos1 = i / qreal(segments);
        qreal pos2 = (i + 1) / qreal(segments);

        QColor color1 = colorForPosition(pos1);
        QColor color2 = colorForPosition(pos2);

        // Create two triangles for this segment
        QSGGeometry::TexturedPoint2D v1, v2, v3, v4;

        v1.set(x1, y, color1.redF(), color1.greenF());
        v2.set(x2, y, color2.redF(), color2.greenF());
        v3.set(x1, y + underlineHeight, color1.redF(), color1.greenF());
        v4.set(x2, y + underlineHeight, color2.redF(), color2.greenF());

        allVertices.append(v1);
        allVertices.append(v2);
        allVertices.append(v3);
        allVertices.append(v4);
    }

    // Create geometry with all vertices
    QSGGeometry *newGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), allVertices.size());
    QSGGeometry::TexturedPoint2D *newVertices = newGeometry->vertexDataAsTexturedPoint2D();

    for (int i = 0; i < allVertices.size(); ++i) {
        newVertices[i] = allVertices[i];
    }

    newGeometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);
    node->setGeometry(newGeometry);
    node->setFlag(QSGNode::OwnsGeometry);
}

QColor DeclarativeUnderline::colorForPosition(qreal position) const
{
    QColor startColor, endColor;

    switch (m_horizontalAlignment) {
        case Qt::AlignLeft:
            startColor = m_primaryColor;
            endColor = m_secondaryColor;
            break;
        case Qt::AlignRight:
            startColor = m_secondaryColor;
            endColor = m_primaryColor;
            break;
        case Qt::AlignHCenter:
            // Symmetric gradient from center to edges
            if (position <= 0.5) {
                startColor = m_primaryColor;
                endColor = m_secondaryColor;
                position = position * 2.0; // Map 0-0.5 to 0-1
            } else {
                startColor = m_secondaryColor;
                endColor = m_primaryColor;
                position = (position - 0.5) * 2.0; // Map 0.5-1 to 0-1
            }
            break;
        default:
            startColor = m_primaryColor;
            endColor = m_secondaryColor;
            break;
    }

    // Linear interpolation between colors
    return QColor(
        startColor.red() + (endColor.red() - startColor.red()) * position,
        startColor.green() + (endColor.green() - startColor.green()) * position,
        startColor.blue() + (endColor.blue() - startColor.blue()) * position,
        startColor.alpha() + (endColor.alpha() - startColor.alpha()) * position
    );
}
