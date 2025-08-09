// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativeutil.h"
#include <QDate>
#include <QMetaObject>
#include <QMetaMethod>
#include <QQmlEngine>
#include <QQmlContext>

DeclarativeUtil *DeclarativeUtil::g_instance = nullptr;

DeclarativeUtil::DeclarativeUtil(QObject *parent)
    : QObject(parent)
{
}

QVariantList DeclarativeUtil::weekNumberList(int year, int month, int day, int amount)
{
    QVariantList result;
    QDate startDate(year, month, day);

    if (!startDate.isValid()) {
        return result;
    }

    for (int i = 0; i < qAbs(amount); ++i) {
        QDate currentDate;
        if (amount > 0) {
            currentDate = startDate.addDays(i * 7);
        } else {
            currentDate = startDate.addDays(-i * 7);
        }

        int weekNumber = currentDate.weekNumber();
        result.append(weekNumber);
    }

    return result;
}

void DeclarativeUtil::asyncInvoke(const QVariant &callable)
{
    if (!callable.isValid()) {
        return;
    }

    // Check if it's a callable (function)
    if (callable.canConvert<QJSValue>()) {
        QJSValue jsValue = callable.value<QJSValue>();
        if (jsValue.isCallable()) {
            // Queue the callable for execution using QMetaObject::invokeMethod
            QMetaObject::invokeMethod(this, [jsValue]() {
                // Create a copy of the JS value for the lambda
                QJSValue copy = jsValue;
                copy.call();
            }, Qt::QueuedConnection);
        }
    }
}

bool DeclarativeUtil::instanceOf(QObject *object, const QString &className)
{
    if (!object) {
        return false;
    }

    const QMetaObject *metaObject = object->metaObject();

    // Check the object's class name
    if (metaObject->className() == className) {
        return true;
    }

    // Check superclasses
    while (metaObject) {
        if (metaObject->className() == className) {
            return true;
        }
        metaObject = metaObject->superClass();
    }

    return false;
}

DeclarativeUtil *DeclarativeUtil::instance()
{
    if (!g_instance) {
        g_instance = new DeclarativeUtil();
    }
    return g_instance;
}
