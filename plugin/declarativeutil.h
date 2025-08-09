// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEUTIL_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEUTIL_H

#include <QObject>
#include <QVariantList>
#include <QDate>

class DeclarativeUtil : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeUtil(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList weekNumberList(int year, int month, int day, int amount);
    Q_INVOKABLE void asyncInvoke(const QVariant &callable);
    Q_INVOKABLE bool instanceOf(QObject *object, const QString &className);

    static DeclarativeUtil *instance();

private:
    static DeclarativeUtil *g_instance;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEUTIL_H
