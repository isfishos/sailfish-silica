// SPDX-License-Identifier: LGPL-2.1-only

#include "autoscrollcontroller.h"
#include <QQuickItem>

AutoScrollController::AutoScrollController(QObject *parent)
    : QObject(parent)
{
}

AutoScrollController *AutoScrollController::qmlAttachedProperties(QObject *target)
{
    return new AutoScrollController(target);
}

QQmlListProperty<QObject> AutoScrollController::data()
{
    auto append = [](QQmlListProperty<QObject> *prop, QObject *obj) {
        auto self = static_cast<AutoScrollController*>(prop->object);
        self->appendChild(obj);
    };
    auto count = [](QQmlListProperty<QObject> *prop) {
        auto self = static_cast<AutoScrollController*>(prop->object);
        return self->childCount();
    };
    auto at = [](QQmlListProperty<QObject> *prop, int idx) -> QObject* {
        auto self = static_cast<AutoScrollController*>(prop->object);
        return self->childAt(idx);
    };
    auto clear = [](QQmlListProperty<QObject> *prop) {
        auto self = static_cast<AutoScrollController*>(prop->object);
        self->clearChildren();
    };
    return QQmlListProperty<QObject>(this, nullptr, append, count, at, clear);
}
