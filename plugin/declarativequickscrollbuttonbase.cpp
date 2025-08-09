// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativequickscrollbuttonbase.h"
#include <QMouseEvent>
#include <QTouchEvent>
#include <QQuickWindow>

DeclarativeQuickScrollButtonBase::DeclarativeQuickScrollButtonBase(QQuickItem *parent)
    : QQuickItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptTouchEvents(true);
}

void DeclarativeQuickScrollButtonBase::setFlickable(QQuickItem *flickable)
{
    if (m_flickable != flickable) {
        m_flickable = flickable;
        emit flickableChanged();
    }
}

void DeclarativeQuickScrollButtonBase::mousePressEvent(QMouseEvent *event)
{
    handlePress(event->pos());
    event->accept();
}

void DeclarativeQuickScrollButtonBase::mouseMoveEvent(QMouseEvent *event)
{
    handleMove(event->pos());
    event->accept();
}

void DeclarativeQuickScrollButtonBase::mouseReleaseEvent(QMouseEvent *event)
{
    handleRelease();
    event->accept();
}

void DeclarativeQuickScrollButtonBase::touchEvent(QTouchEvent *event)
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

void DeclarativeQuickScrollButtonBase::handlePress(const QPointF &pos)
{
    m_pressPos = pos;
    m_lastPos = pos;
    m_pressed = true;
    m_tracking = true;
    emit pressedChanged();
}

void DeclarativeQuickScrollButtonBase::handleMove(const QPointF &pos)
{
    if (!m_tracking) {
        return;
    }

    m_lastPos = pos;

    // Check if movement exceeds tolerance
    QPointF delta = pos - m_pressPos;
    if (delta.manhattanLength() > CLICK_TOLERANCE) {
        // Movement detected, keep pressed but don't click on release
        m_tracking = false;
    }
}

void DeclarativeQuickScrollButtonBase::handleRelease()
{
    if (m_pressed) {
        m_pressed = false;
        emit pressedChanged();

        // Only emit clicked if we're still within tolerance
        if (m_tracking && isWithinTolerance(m_lastPos)) {
            emit clicked();
        }

        m_tracking = false;
    }
}

bool DeclarativeQuickScrollButtonBase::isWithinTolerance(const QPointF &pos)
{
    QPointF delta = pos - m_pressPos;
    return delta.manhattanLength() <= CLICK_TOLERANCE;
}
