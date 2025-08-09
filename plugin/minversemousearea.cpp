// SPDX-License-Identifier: LGPL-2.1-only

#include "minversemousearea.h"
#include <QMouseEvent>
#include <QTouchEvent>
#include <QQuickWindow>

MInverseMouseArea::MInverseMouseArea(QQuickItem *parent)
    : QQuickItem(parent)
{
}

void MInverseMouseArea::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        emit enabledChanged();

        if (m_enabled) {
            installEventFilter();
        } else {
            removeEventFilter();
        }
    }
}

void MInverseMouseArea::setStealPress(bool steal)
{
    if (m_stealPress != steal) {
        m_stealPress = steal;
        emit stealPressChanged();
    }
}

void MInverseMouseArea::cancelTouch()
{
    m_pressed = false;
    // TODO: Synthesize canceled state if needed
}

void MInverseMouseArea::itemChange(ItemChange change, const ItemChangeData &value)
{
    if (change == ItemSceneChange && m_enabled) {
        removeEventFilter();
        installEventFilter();
    }

    QQuickItem::itemChange(change, value);
}

void MInverseMouseArea::installEventFilter()
{
    if (QQuickWindow *window = this->window()) {
        m_window = window;
        window->installEventFilter(this);
    }
}

void MInverseMouseArea::removeEventFilter()
{
    if (m_window) {
        m_window->removeEventFilter(this);
        m_window = nullptr;
    }
}

bool MInverseMouseArea::eventFilter(QObject *obj, QEvent *event)
{
    if (!m_enabled || m_blockRecursion) {
        return false;
    }

    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF pos = mouseEvent->pos();

            if (isPointOutside(pos)) {
                m_pressPos = pos;
                m_pressed = true;
                emit pressedOutside(pos.x(), pos.y());

                if (m_stealPress) {
                    return true; // Accept the event
                }
            }
            break;
        }
        case QEvent::MouseButtonRelease: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF pos = mouseEvent->pos();

            if (m_pressed && isPointOutside(pos)) {
                emit clickedOutside(pos.x(), pos.y());
                m_pressed = false;

                if (m_stealPress) {
                    return true; // Accept the event
                }
            }
            break;
        }
        case QEvent::TouchBegin: {
            QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
            if (!touchEvent->touchPoints().isEmpty()) {
                QPointF pos = touchEvent->touchPoints().first().pos();

                if (isPointOutside(pos)) {
                    m_pressPos = pos;
                    m_pressed = true;
                    emit pressedOutside(pos.x(), pos.y());

                    if (m_stealPress) {
                        return true; // Accept the event
                    }
                }
            }
            break;
        }
        case QEvent::TouchEnd: {
            QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
            if (!touchEvent->touchPoints().isEmpty()) {
                QPointF pos = touchEvent->touchPoints().first().pos();

                if (m_pressed && isPointOutside(pos)) {
                    emit clickedOutside(pos.x(), pos.y());
                    m_pressed = false;

                    if (m_stealPress) {
                        return true; // Accept the event
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    return false;
}

bool MInverseMouseArea::isPointOutside(const QPointF &pos) const
{
    // Map the point to this item's coordinate system
    QPointF mappedPos = mapFromScene(pos);

    // Check if the point is outside the item's bounds
    return !boundingRect().contains(mappedPos);
}
