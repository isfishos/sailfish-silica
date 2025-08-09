// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativebounceeffect.h"
#include <private/qquickflickable_p.h>

DeclarativeBounceEffect::DeclarativeBounceEffect(QObject *parent)
    : QObject(parent)
{
}

bool DeclarativeBounceEffect::active() const
{
    return m_active;
}

qreal DeclarativeBounceEffect::difference() const
{
    return m_difference;
}

QQuickFlickable *DeclarativeBounceEffect::flickable() const
{
    return m_flickable;
}

void DeclarativeBounceEffect::setFlickable(QQuickFlickable *f)
{
    if (m_flickable == f)
        return;
    m_flickable = f;
    Q_EMIT flickableChanged();
}

QQmlListProperty<QObject> DeclarativeBounceEffect::data()
{
    auto append = [](QQmlListProperty<QObject> *p, QObject *o) {
        static_cast<DeclarativeBounceEffect*>(p->object)->m_data.append(o);
    };
    auto count = [](QQmlListProperty<QObject> *p) {
        return static_cast<DeclarativeBounceEffect*>(p->object)->m_data.count();
    };
    auto at = [](QQmlListProperty<QObject> *p, int i) {
        return static_cast<DeclarativeBounceEffect*>(p->object)->m_data.value(i);
    };
    auto clear = [](QQmlListProperty<QObject> *p) {
        static_cast<DeclarativeBounceEffect*>(p->object)->m_data.clear();
    };
    return QQmlListProperty<QObject>(this, nullptr, append, count, at, clear);
}

void DeclarativeBounceEffect::handlePress(const QPointF &pos)
{
    m_startPos = pos;
}

void DeclarativeBounceEffect::handleMove(const QPointF &pos)
{
    if (!m_flickable)
        return;

    const QPointF delta = pos - m_startPos;
    // Simple vertical overscroll difference estimate
    qreal diff = 0.0;
    if (m_flickable->contentY() < 0)
        diff = -m_flickable->contentY();
    else if (m_flickable->contentY() > m_flickable->contentHeight() - m_flickable->height())
        diff = m_flickable->contentY() - (m_flickable->contentHeight() - m_flickable->height());
    Q_UNUSED(delta)

    const bool nowActive = diff > 0.0;
    if (m_active != nowActive) { m_active = nowActive; Q_EMIT activeChanged(); }
    if (!qFuzzyCompare(m_difference, diff)) { m_difference = diff; Q_EMIT differenceChanged(); }
}

void DeclarativeBounceEffect::handleRelease()
{
    if (m_active) { m_active = false; Q_EMIT activeChanged(); }
    if (!qFuzzyIsNull(m_difference)) { m_difference = 0.0; Q_EMIT differenceChanged(); }
}
