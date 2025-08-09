// SPDX-License-Identifier: LGPL-2.1-only

#include "dragdetectoritem.h"
#include <QMouseEvent>
#include <QTouchEvent>
#include <private/qquickflickable_p.h>

DragDetectorItem::DragDetectorItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptTouchEvents(true);
}

void DragDetectorItem::setFlickable(QQuickItem *flickable)
{
    if (m_flickable != flickable) {
        m_flickable = flickable;
        emit flickableChanged();
    }
}

void DragDetectorItem::reset()
{
    m_horizontalDragUnused = false;
    m_verticalDragUnused = false;
    m_dragging = false;
    emit horizontalDragUnusedChanged();
    emit verticalDragUnusedChanged();
}

void DragDetectorItem::mousePressEvent(QMouseEvent *event)
{
    handlePress(event->pos());
    event->accept();
}

void DragDetectorItem::mouseMoveEvent(QMouseEvent *event)
{
    handleMove(event->pos());
    event->accept();
}

void DragDetectorItem::mouseReleaseEvent(QMouseEvent *event)
{
    handleRelease();
    event->accept();
}

void DragDetectorItem::touchEvent(QTouchEvent *event)
{
    switch (event->type()) {
        case QEvent::TouchBegin: {
            if (!event->touchPoints().isEmpty()) {
                handlePress(event->touchPoints().first().pos());
            }
            event->accept();
            break;
        }
        case QEvent::TouchUpdate: {
            if (!event->touchPoints().isEmpty()) {
                handleMove(event->touchPoints().first().pos());
            }
            event->accept();
            break;
        }
        case QEvent::TouchEnd: {
            handleRelease();
            event->accept();
            break;
        }
        default:
            break;
    }
}

void DragDetectorItem::handlePress(const QPointF &pos)
{
    m_pressPos = pos;
    m_lastPos = pos;
    m_dragging = false;
    reset();
}

void DragDetectorItem::handleMove(const QPointF &pos)
{
    m_lastPos = pos;

    // Check if we've exceeded the drag threshold
    QPointF delta = pos - m_pressPos;
    if (delta.manhattanLength() > DRAG_THRESHOLD) {
        m_dragging = true;
        updateDragUnused();
    }
}

void DragDetectorItem::handleRelease()
{
    m_dragging = false;
}

void DragDetectorItem::updateDragUnused()
{
    if (!m_dragging || !m_flickable) {
        return;
    }

    QPointF delta = m_lastPos - m_pressPos;
    bool horizontalDrag = qAbs(delta.x()) > qAbs(delta.y());
    bool verticalDrag = qAbs(delta.y()) > qAbs(delta.x());

    // Check if the flickable can scroll in the dragged direction
    bool canScrollHorizontally = false;
    bool canScrollVertically = false;

    if (QQuickFlickable *flickable = qobject_cast<QQuickFlickable*>(m_flickable)) {
        // Check if content is larger than viewport
        qreal contentWidth = flickable->contentWidth();
        qreal contentHeight = flickable->contentHeight();
        qreal viewportWidth = flickable->width();
        qreal viewportHeight = flickable->height();

        canScrollHorizontally = contentWidth > viewportWidth;
        canScrollVertically = contentHeight > viewportHeight;

        // Check if flicking is enabled
        QQuickFlickable::FlickableDirection direction = flickable->flickableDirection();
        if (!(direction & QQuickFlickable::HorizontalFlick)) {
            canScrollHorizontally = false;
        }
        if (!(direction & QQuickFlickable::VerticalFlick)) {
            canScrollVertically = false;
        }
    }

    // Update unused flags
    bool newHorizontalDragUnused = horizontalDrag && !canScrollHorizontally;
    bool newVerticalDragUnused = verticalDrag && !canScrollVertically;

    if (m_horizontalDragUnused != newHorizontalDragUnused) {
        m_horizontalDragUnused = newHorizontalDragUnused;
        emit horizontalDragUnusedChanged();
    }

    if (m_verticalDragUnused != newVerticalDragUnused) {
        m_verticalDragUnused = newVerticalDragUnused;
        emit verticalDragUnusedChanged();
    }
}
