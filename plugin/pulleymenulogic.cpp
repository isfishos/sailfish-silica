// SPDX-License-Identifier: LGPL-2.1-only

#include "pulleymenulogic.h"
#include <QPropertyAnimation>
#include <private/qquickflickable_p.h>

PulleyMenuLogic::PulleyMenuLogic(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &PulleyMenuLogic::onTimerTimeout);
}

void PulleyMenuLogic::setPullDownType(bool pullDown)
{
    if (m_pullDownType != pullDown) {
        m_pullDownType = pullDown;
        emit pullDownTypeChanged();
        updateDragDistance();
    }
}

void PulleyMenuLogic::setFlickable(QObject *flickable)
{
    if (m_flickable != flickable) {
        m_flickable = flickable;
        emit flickableChanged();
        connectToFlickable();
        updateDragDistance();
    }
}

void PulleyMenuLogic::componentComplete()
{
    connectToFlickable();
}

bool PulleyMenuLogic::outOfBounds()
{
    if (!m_flickable) {
        return false;
    }

    QQuickFlickable *flick = qobject_cast<QQuickFlickable*>(m_flickable);
    if (!flick) {
        return false;
    }

    qreal contentY = flick->property("contentY").toReal();
    qreal threshold = 50.0; // Configurable threshold

    if (m_pullDownType) {
        return contentY < -threshold;
    } else {
        qreal maxContentY = flick->property("contentHeight").toReal() - flick->height();
        return contentY > maxContentY + threshold;
    }
}

void PulleyMenuLogic::monitorFlick()
{
    if (!m_flickable) {
        return;
    }

    m_monitoring = true;
    m_timer.start(16); // ~60fps
}

void PulleyMenuLogic::onFlickablePropertyChanged()
{
    updateDragDistance();
}

void PulleyMenuLogic::onTimerTimeout()
{
    if (!m_monitoring || !m_flickable) {
        m_timer.stop();
        m_monitoring = false;
        return;
    }

    if (outOfBounds()) {
        // Emit animation request
        emit animateFlick(300, m_pullDownType ? 0.0 : 100.0);
        m_timer.stop();
        m_monitoring = false;
    }
}

void PulleyMenuLogic::updateDragDistance()
{
    if (!m_flickable) {
        if (m_dragDistance != 0.0) {
            m_dragDistance = 0.0;
            emit dragDistanceChanged();
        }
        return;
    }

    QQuickFlickable *flick = qobject_cast<QQuickFlickable*>(m_flickable);
    if (!flick) {
        return;
    }

    qreal contentY = flick->property("contentY").toReal();
    qreal newDragDistance = 0.0;

    if (m_pullDownType) {
        if (contentY < 0) {
            newDragDistance = -contentY;
        }
    } else {
        qreal maxContentY = flick->property("contentHeight").toReal() - flick->height();
        if (contentY > maxContentY) {
            newDragDistance = contentY - maxContentY;
        }
    }

    if (!qFuzzyCompare(m_dragDistance, newDragDistance)) {
        m_dragDistance = newDragDistance;
        emit dragDistanceChanged();
    }
}

void PulleyMenuLogic::connectToFlickable()
{
    if (!m_flickable) {
        return;
    }

    // Connect to relevant flickable properties
    QQuickFlickable *flick = qobject_cast<QQuickFlickable*>(m_flickable);
    if (flick) {
        connect(flick, &QQuickFlickable::contentYChanged, this, &PulleyMenuLogic::onFlickablePropertyChanged);
        connect(flick, &QQuickFlickable::draggingChanged, this, &PulleyMenuLogic::onFlickablePropertyChanged);
    }
}
