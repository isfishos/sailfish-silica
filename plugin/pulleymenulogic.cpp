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

    // Compute content offset relative to the flickable's originY so that
    // pull-down/push-up distance matches the QML-side calculations which
    // use flickable.originY as the baseline.
    qreal contentY = flick->property("contentY").toReal();
    qreal originY = flick->property("originY").toReal();
    qreal offset = contentY - originY;
    qreal threshold = 50.0; // Configurable threshold

    if (m_pullDownType) {
        // Pull-down: offset will be negative when pulled down from origin
        return offset < -threshold;
    } else {
        qreal contentHeight = flick->property("contentHeight").toReal();
        qreal maxContentY = contentHeight - flick->height();
        qreal maxOffset = maxContentY - originY;
        return offset > maxOffset + threshold;
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
    qreal originY = flick->property("originY").toReal();
    qreal offset = contentY - originY;
    qreal newDragDistance = 0.0;

    if (m_pullDownType) {
        // When pulled down, offset is negative; drag distance is positive magnitude
        if (offset < 0) {
            newDragDistance = -offset;
        }
    } else {
        qreal contentHeight = flick->property("contentHeight").toReal();
        qreal maxContentY = contentHeight - flick->height();
        qreal maxOffset = maxContentY - originY;
        if (offset > maxOffset) {
            newDragDistance = offset - maxOffset;
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

        // Try to cache the associated PulleyMenu instance which the QML side
        // assigns to flickable.pullDownMenu or flickable.pushUpMenu.
        QVariant pd = flick->property("pullDownMenu");
        if (pd.isValid() && pd.canConvert<QObject*>()) {
            m_menu = pd.value<QObject*>();
        } else {
            QVariant pu = flick->property("pushUpMenu");
            if (pu.isValid() && pu.canConvert<QObject*>())
                m_menu = pu.value<QObject*>();
        }
    }
}

void PulleyMenuLogic::onFlickablePropertyChanged()
{
    if (!m_flickable)
        return;

    // Prevent reentrant handling
    if (m_inContentYHandler)
        return;
    m_inContentYHandler = true;

    // If the QML menu indicates it's changing layout, skip activation handling
    if (m_menu && m_menu->property("_changingListView").toBool()) {
        m_inContentYHandler = false;
        return;
    }

    // Update drag distance and notify listeners
    updateDragDistance();

    // Determine whether the menu should be active based on the flickable position
    bool wasActive = false;
    if (m_menu)
        wasActive = m_menu->property("active").toBool();

    bool shouldActivate = false;
    if (m_menu && m_flickable) {
        bool enabled = m_menu->property("enabled").toBool();
        bool visible = m_menu->property("visible").toBool();
        qreal contentY = m_flickable->property("contentY").toReal();
        qreal inactivePos = m_menu->property("_inactivePosition").toReal();

        if (enabled && visible) {
            if (m_pullDownType) {
                shouldActivate = contentY < inactivePos;
            } else {
                shouldActivate = contentY > inactivePos;
            }
        }
    }

    // Only activate if the user is actively dragging
    bool dragging = m_flickable ? m_flickable->property("dragging").toBool() : false;
    bool newActive = wasActive;
    if (shouldActivate && dragging) {
        newActive = true;
    } else if (!shouldActivate) {
        newActive = false;
    }

    if (m_menu && newActive != wasActive) {
        m_menu->setProperty("active", newActive);
    }

    // Check for final position reached and notify
    if (m_menu && m_flickable) {
        qreal contentY = m_flickable->property("contentY").toReal();
        qreal finalPos = m_menu->property("_finalPosition").toReal();
        if (qAbs(contentY - finalPos) < 1.0) {
            if (!m_atFinalPosition) {
                m_atFinalPosition = true;
                emit finalPositionReached();
            }
        } else {
            m_atFinalPosition = false;
        }
    }

    // Notify listeners that dragDistance changed
    emit dragDistanceChanged();

    m_firstMovement = false;
    m_inContentYHandler = false;
}

#include "moc_pulleymenulogic.cpp"