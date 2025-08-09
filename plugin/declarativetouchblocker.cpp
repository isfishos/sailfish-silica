// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativetouchblocker.h"
#include <QMouseEvent>
#include <QTouchEvent>
#include <QQuickWindow>

DeclarativeTouchBlocker::DeclarativeTouchBlocker(QQuickItem *parent)
    : QQuickItem(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptTouchEvents(true);
}

void DeclarativeTouchBlocker::setTarget(QQuickItem *target)
{
    if (m_target != target) {
        removeEventFilter();
        m_target = target;
        installEventFilter();
        emit targetChanged();
    }
}

void DeclarativeTouchBlocker::mousePressEvent(QMouseEvent *event)
{
    // Block the event by accepting it and not calling base implementation
    event->accept();
}

void DeclarativeTouchBlocker::mouseMoveEvent(QMouseEvent *event)
{
    // Block the event by accepting it and not calling base implementation
    event->accept();
}

void DeclarativeTouchBlocker::mouseReleaseEvent(QMouseEvent *event)
{
    // Block the event by accepting it and not calling base implementation
    event->accept();
}

void DeclarativeTouchBlocker::touchEvent(QTouchEvent *event)
{
    // Block all touch events by accepting them and not calling base implementation
    event->accept();
}

void DeclarativeTouchBlocker::installEventFilter()
{
    if (m_target && !m_eventFilterInstalled) {
        // Install event filter on the target's window
        if (QQuickWindow *window = m_target->window()) {
            window->installEventFilter(this);
            m_eventFilterInstalled = true;
        }
    }
}

void DeclarativeTouchBlocker::removeEventFilter()
{
    if (m_eventFilterInstalled) {
        if (QQuickWindow *window = m_target ? m_target->window() : nullptr) {
            window->removeEventFilter(this);
        }
        m_eventFilterInstalled = false;
    }
}
