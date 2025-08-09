// SPDX-License-Identifier: LGPL-2.1-only

#include "drawingarea.h"
#include <QMouseEvent>
#include <QTouchEvent>
#include <QVector2D>

DrawingArea::DrawingArea(QQuickItem *parent)
    : LineItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptTouchEvents(true);
}

void DrawingArea::setThreshold(qreal threshold)
{
    if (m_threshold != threshold) {
        m_threshold = threshold;
        emit thresholdChanged();
    }
}

void DrawingArea::setMask(QQuickItem *mask)
{
    if (m_mask != mask) {
        m_mask = mask;
        emit maskChanged();
    }
}

void DrawingArea::setMaximumStrokeCount(int count)
{
    if (m_maximumStrokeCount != count) {
        m_maximumStrokeCount = count;
        emit maximumStrokeCountChanged();
    }
}

void DrawingArea::clear()
{
    m_strokes.clear();
    m_currentStroke.clear();
    m_drawing = false;

    // Clear all line series
    for (int i = 0; i < m_strokes.size(); ++i) {
        setPoints(i, QVector<QVector2D>());
    }
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    handlePress(event->pos());
    event->accept();
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    handleMove(event->pos());
    event->accept();
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    handleRelease();
    event->accept();
}

void DrawingArea::touchEvent(QTouchEvent *event)
{
    switch (event->type()) {
        case QEvent::TouchBegin: {
            if (!event->touchPoints().isEmpty()) {
                const QTouchEvent::TouchPoint &point = event->touchPoints().first();
                m_touchId = point.id();
                handlePress(point.pos());
            }
            event->accept();
            break;
        }
        case QEvent::TouchUpdate: {
            for (const QTouchEvent::TouchPoint &point : event->touchPoints()) {
                if (point.id() == m_touchId) {
                    handleMove(point.pos());
                    break;
                }
            }
            event->accept();
            break;
        }
        case QEvent::TouchEnd: {
            handleRelease();
            m_touchId = -1;
            event->accept();
            break;
        }
        default:
            break;
    }
}

void DrawingArea::handlePress(const QPointF &pos)
{
    if (!isPointInMask(pos)) {
        return;
    }

    m_currentStroke.clear();
    m_lastPoint = pos;
    m_drawing = true;

    // Emit arc started signal
    emit arcStarted(pos.x(), pos.y());
}

void DrawingArea::handleMove(const QPointF &pos)
{
    if (!m_drawing || !isPointInMask(pos)) {
        return;
    }

    // Check if distance exceeds threshold
    QPointF delta = pos - m_lastPoint;
    if (delta.manhattanLength() > m_threshold) {
        m_currentStroke.append(pos);
        m_lastPoint = pos;

        // Emit arc point added signal
        emit arcPointAdded(pos.x(), pos.y());
    }
}

void DrawingArea::handleRelease()
{
    if (!m_drawing) {
        return;
    }

    m_drawing = false;

    if (m_currentStroke.size() >= 2) {
        // Add the stroke
        addStroke(m_currentStroke);
        emit arcFinished();
    } else {
        // Too few points, cancel the stroke
        emit arcCanceled();
    }

    m_currentStroke.clear();
}

bool DrawingArea::isPointInMask(const QPointF &pos) const
{
    if (!m_mask) {
        return true;
    }

    // Map the point to mask coordinates
    QPointF mappedPos = mapToItem(m_mask, pos);

    // Check if the point is within the mask bounds
    return m_mask->boundingRect().contains(mappedPos) && m_mask->isVisible();
}

void DrawingArea::addStroke(const QVector<QPointF> &points)
{
    // Convert QPointF to QVector2D
    QVector<QVector2D> linePoints;
    linePoints.reserve(points.size());

    for (const QPointF &point : points) {
        linePoints.append(QVector2D(point));
    }

    // Add the stroke to our collection
    m_strokes.append(points);

    // Remove oldest strokes if we exceed maximum count
    while (m_strokes.size() > m_maximumStrokeCount) {
        m_strokes.removeFirst();
    }

    // Update the line series
    setPoints(0, linePoints);
}
