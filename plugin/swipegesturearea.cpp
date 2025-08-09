// SPDX-License-Identifier: LGPL-2.1-only

#include "swipegesturearea.h"
#include <QMouseEvent>
#include <QtMath>

SwipeGestureArea::SwipeGestureArea(QQuickItem *parent)
    : QQuickItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

void SwipeGestureArea::setSwipeEnabled(bool enabled)
{
    if (m_swipeEnabled != enabled) {
        m_swipeEnabled = enabled;
        emit swipeEnabledChanged();
    }
}

void SwipeGestureArea::setThresholdX(int threshold)
{
    if (m_thresholdX != threshold) {
        m_thresholdX = threshold;
        emit thresholdXChanged();
    }
}

void SwipeGestureArea::setThresholdY(int threshold)
{
    if (m_thresholdY != threshold) {
        m_thresholdY = threshold;
        emit thresholdYChanged();
    }
}

void SwipeGestureArea::setAllowedDirections(Directions directions)
{
    if (m_allowedDirections != directions) {
        m_allowedDirections = directions;
        emit allowedDirectionsChanged();
    }
}

void SwipeGestureArea::setForceDirections(Directions directions)
{
    if (m_forceDirections != directions) {
        m_forceDirections = directions;
        emit forceDirectionsChanged();
    }
}

void SwipeGestureArea::endGesture()
{
    resetGesture();
}

void SwipeGestureArea::mousePressEvent(QMouseEvent *event)
{
    if (!m_swipeEnabled) {
        QQuickItem::mousePressEvent(event);
        return;
    }

    m_startPos = event->pos();
    m_lastPos = m_startPos;
    m_pressed = true;
    resetGesture();
    event->accept();
}

void SwipeGestureArea::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_swipeEnabled || !m_pressed) {
        QQuickItem::mouseMoveEvent(event);
        return;
    }

    QPointF currentPos = event->pos();
    QPointF delta = currentPos - m_startPos;
    qreal dx = delta.x();
    qreal dy = delta.y();

    if (!m_gestureInProgress) {
        // Check if we've exceeded thresholds
        if (qAbs(dx) > m_thresholdX || qAbs(dy) > m_thresholdY) {
            Direction candidateDirection = determineDirection(dx, dy);

            if (candidateDirection != None && isDirectionAllowed(candidateDirection)) {
                m_direction = candidateDirection;
                m_gestureInProgress = true;
                emit directionChanged();
                emit gestureInProgressChanged();
            }
        }
    }

    if (m_gestureInProgress) {
        // Update swipe amount based on direction
        switch (m_direction) {
        case Left:
        case Right:
            m_swipeAmount = static_cast<int>(dx);
            break;
        case Up:
        case Down:
            m_swipeAmount = static_cast<int>(dy);
            break;
        default:
            break;
        }
        emit swipeAmountChanged();
    }

    m_lastPos = currentPos;
    event->accept();
}

void SwipeGestureArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_swipeEnabled) {
        QQuickItem::mouseReleaseEvent(event);
        return;
    }

    if (m_gestureInProgress) {
        resetGesture();
    }

    m_pressed = false;
    event->accept();
}

bool SwipeGestureArea::childMouseEventFilter(QQuickItem *item, QEvent *event)
{
    Q_UNUSED(item)

    if (!m_swipeEnabled) {
        return false;
    }

    switch (event->type()) {
    case QEvent::MouseButtonPress:
        mousePressEvent(static_cast<QMouseEvent*>(event));
        return true;
    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent*>(event));
        return true;
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent*>(event));
        return true;
    default:
        break;
    }

    return false;
}

void SwipeGestureArea::resetGesture()
{
    if (m_gestureInProgress) {
        m_gestureInProgress = false;
        emit gestureInProgressChanged();
    }

    if (m_direction != None) {
        m_direction = None;
        emit directionChanged();
    }

    if (m_swipeAmount != 0) {
        m_swipeAmount = 0;
        emit swipeAmountChanged();
    }
}

SwipeGestureArea::Direction SwipeGestureArea::determineDirection(qreal dx, qreal dy)
{
    qreal absDx = qAbs(dx);
    qreal absDy = qAbs(dy);

    if (absDx > absDy) {
        // Horizontal movement
        if (dx > 0) {
            return Right;
        } else {
            return Left;
        }
    } else {
        // Vertical movement
        if (dy > 0) {
            return Down;
        } else {
            return Up;
        }
    }
}

bool SwipeGestureArea::isDirectionAllowed(Direction direction) const
{
    if (m_forceDirections != None) {
        return (m_forceDirections & direction) == direction;
    }

    return (m_allowedDirections & direction) == direction;
}
