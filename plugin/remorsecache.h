// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_REMORSECACHE_H
#define SAILFISH_SILICA_PLUGIN_REMORSECACHE_H

#include <QObject>
#include <QPointer>

class QQuickItem;

class RemorseCache : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *item READ item WRITE setItem NOTIFY itemChanged)

public:
    explicit RemorseCache(QObject *parent = nullptr);

    QQuickItem *item() const { return m_item; }
    void setItem(QQuickItem *item);

    static RemorseCache *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void itemChanged();

private:
    QPointer<QQuickItem> m_item;
};

#endif // SAILFISH_SILICA_PLUGIN_REMORSECACHE_H
