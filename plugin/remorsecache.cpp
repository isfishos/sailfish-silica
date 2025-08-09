// SPDX-License-Identifier: LGPL-2.1-only

#include "remorsecache.h"
#include <QQuickItem>

RemorseCache::RemorseCache(QObject *parent)
    : QObject(parent)
{
}

void RemorseCache::setItem(QQuickItem *item)
{
    if (m_item != item) {
        m_item = item;
        emit itemChanged();
    }
}

RemorseCache *RemorseCache::qmlAttachedProperties(QObject *object)
{
    return new RemorseCache(object);
}
