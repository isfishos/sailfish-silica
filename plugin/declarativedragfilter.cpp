// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativedragfilter.h"
#include <QQuickItem>
#include <QQuickWindow>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QQmlEngine>
#include <QQmlContext>

DeclarativeDragFilter::DeclarativeDragFilter(QObject *parent)
    : QObject(parent)
{
}

void DeclarativeDragFilter::setScreenMargin(qreal margin)
{
    if (m_screenMargin != margin) {
        m_screenMargin = margin;
        emit screenMarginChanged();
    }
}

void DeclarativeDragFilter::setOrientations(Qt::Orientations orientations)
{
    if (m_orientations != orientations) {
        m_orientations = orientations;
        emit orientationsChanged();
    }
}

void DeclarativeDragFilter::begin(qreal x, qreal y)
{
    resetState();

    m_pressPos = QPointF(x, y);
    m_targetItem = qobject_cast<QQuickItem*>(parent());

    if (m_targetItem) {
        m_window = m_targetItem->window();
        if (m_window) {
            m_window->installEventFilter(this);
        }
    }

    // Check if press is in screen margin
    if (isInScreenMargin(m_pressPos)) {
        m_canceled = true;
        emit canceledChanged();
    }

    m_tracking = true;
}

void DeclarativeDragFilter::end()
{
    if (m_window) {
        m_window->removeEventFilter(this);
        m_window = nullptr;
    }
    resetState();
}

void DeclarativeDragFilter::cancelClick()
{
    m_canceled = true;
    emit canceledChanged();

    // Synthesize an accepted event to stop propagation
    if (m_targetItem) {
        QMouseEvent cancelEvent(QEvent::MouseButtonPress, QPointF(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        cancelEvent.setAccepted(true);
    }
}

DeclarativeDragFilter *DeclarativeDragFilter::qmlAttachedProperties(QObject *obj)
{
    // Return one instance per target object
    static QHash<QObject*, DeclarativeDragFilter*> instances;

    if (!instances.contains(obj)) {
        instances[obj] = new DeclarativeDragFilter(obj);
    }

    return instances[obj];
}

bool DeclarativeDragFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (!m_tracking || obj != m_window) {
        return false;
    }

    switch (event->type()) {
        case QEvent::MouseMove: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF currentPos = mouseEvent->pos();

            if (exceedsDragThreshold(currentPos)) {
                m_canceled = true;
                emit canceledChanged();
            }
            break;
        }
        case QEvent::TouchUpdate: {
            QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
            if (!touchEvent->touchPoints().isEmpty()) {
                QPointF currentPos = touchEvent->touchPoints().first().pos();

                if (exceedsDragThreshold(currentPos)) {
                    m_canceled = true;
                    emit canceledChanged();
                }
            }
            break;
        }
        case QEvent::MouseButtonRelease:
        case QEvent::TouchEnd:
            end();
            break;
        default:
            break;
    }

    return false;
}

void DeclarativeDragFilter::resetState()
{
    m_canceled = false;
    m_pressPos = QPointF();
    m_targetItem = nullptr;
    m_tracking = false;
    emit canceledChanged();
}

bool DeclarativeDragFilter::isInScreenMargin(const QPointF &pos)
{
    if (m_screenMargin <= 0.0 || !m_window) {
        return false;
    }

    qreal windowWidth = m_window->width();
    return pos.x() <= m_screenMargin || pos.x() >= (windowWidth - m_screenMargin);
}

bool DeclarativeDragFilter::exceedsDragThreshold(const QPointF &currentPos)
{
    QPointF delta = currentPos - m_pressPos;

    if (m_orientations & Qt::Horizontal && qAbs(delta.x()) > DRAG_THRESHOLD) {
        return true;
    }

    if (m_orientations & Qt::Vertical && qAbs(delta.y()) > DRAG_THRESHOLD) {
        return true;
    }

    return false;
}
