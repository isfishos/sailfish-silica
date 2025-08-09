// SPDX-License-Identifier: LGPL-2.1-only

#include "slideable.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>

Slide::Slide(QObject *parent)
    : QObject(parent)
{
}

void Slide::setView(Slideable *view)
{
    if (m_view != view) {
        m_view = view;
        emit viewChanged();
        emit _viewChanged();
    }
}

void Slide::setBackward(QQuickItem *backward)
{
    if (m_backward != backward) {
        m_backward = backward;
        emit backwardChanged();
    }
}

void Slide::setForward(QQuickItem *forward)
{
    if (m_forward != forward) {
        m_forward = forward;
        emit forwardChanged();
    }
}

void Slide::setOffset(qreal offset)
{
    if (!qFuzzyCompare(m_offset, offset)) {
        m_offset = offset;
        emit offsetChanged();
        emit _offsetChanged();
    }
}

void Slide::setIsCurrent(bool current)
{
    if (m_isCurrent != current) {
        m_isCurrent = current;
        emit isCurrentChanged();
    }
}

void Slide::setIsExposed(bool exposed)
{
    if (m_isExposed != exposed) {
        m_isExposed = exposed;
        emit isExposedChanged();
    }
}

void Slide::setIsFirst(bool first)
{
    if (m_isFirst != first) {
        m_isFirst = first;
        emit isFirstChanged();
    }
}

void Slide::setIsLast(bool last)
{
    if (m_isLast != last) {
        m_isLast = last;
        emit isLastChanged();
    }
}

void Slide::setInCache(bool inCache)
{
    if (m_inCache != inCache) {
        m_inCache = inCache;
        emit inCacheChanged();
    }
}

void Slide::setKeepAlive(bool keepAlive)
{
    if (m_keepAlive != keepAlive) {
        m_keepAlive = keepAlive;
        emit keepAliveChanged();
    }
}

Slide *Slide::qmlAttachedProperties(QObject *object)
{
    return new Slide(object);
}

// Slideable implementation
Slideable::Slideable(QQuickItem *parent)
    : QQuickItem(parent)
{
}

void Slideable::setCurrentItem(QQuickItem *item)
{
    if (m_currentItem != item) {
        QQuickItem *oldItem = m_currentItem;
        m_currentItem = item;

        // Update slide properties for old and new items
        if (oldItem) {
            Slide *oldSlide = qobject_cast<Slide*>(qmlAttachedPropertiesObject<Slide>(oldItem, false));
            if (oldSlide) {
                oldSlide->setIsCurrent(false);
            }
        }

        if (m_currentItem) {
            Slide *newSlide = qobject_cast<Slide*>(qmlAttachedPropertiesObject<Slide>(m_currentItem, false));
            if (newSlide) {
                newSlide->setIsCurrent(true);
                newSlide->setView(this);
            }
        }

        updateSlideProperties();
        emit currentItemChanged();
    }
}

void Slideable::setFlow(Slide::Flow flow)
{
    if (m_flow != flow) {
        m_flow = flow;
        updateSlideProperties();
        emit flowChanged();
    }
}

void Slideable::setCacheSize(int size)
{
    if (m_cacheSize != size) {
        m_cacheSize = size;
        updateCache();
        emit cacheSizeChanged();
    }
}

void Slideable::setCacheExpiry(int expiry)
{
    if (m_cacheExpiry != expiry) {
        m_cacheExpiry = expiry;
        emit cacheExpiryChanged();
    }
}

void Slideable::cache(QQuickItem *item)
{
    if (item && !m_cachedItems.contains(item)) {
        m_cachedItems.append(item);

        Slide *slide = qobject_cast<Slide*>(qmlAttachedPropertiesObject<Slide>(item, false));
        if (slide) {
            slide->setInCache(true);
        }

        updateCache();
    }
}

void Slideable::componentComplete()
{
    QQuickItem::componentComplete();
    updateSlideProperties();
}

void Slideable::itemChange(ItemChange change, const ItemChangeData &data)
{
    QQuickItem::itemChange(change, data);

    if (change == ItemChildAddedChange || change == ItemChildRemovedChange) {
        updateSlideProperties();
    }
}

void Slideable::onCacheExpiry()
{
    evictExpiredItems();
}

void Slideable::updateSlideProperties()
{
    QList<QQuickItem*> children = childItems();

    for (int i = 0; i < children.size(); ++i) {
        QQuickItem *child = children[i];
        Slide *slide = qobject_cast<Slide*>(qmlAttachedPropertiesObject<Slide>(child, false));
        if (slide) {
            slide->setView(this);

            // Set backward/forward items
            if (i > 0) {
                slide->setBackward(children[i - 1]);
            } else {
                slide->setBackward(nullptr);
            }

            if (i < children.size() - 1) {
                slide->setForward(children[i + 1]);
            } else {
                slide->setForward(nullptr);
            }

            // Set first/last flags
            slide->setIsFirst(i == 0);
            slide->setIsLast(i == children.size() - 1);

            // Set exposed flag (simplified - could be more sophisticated)
            bool exposed = (child == m_currentItem || m_cachedItems.contains(child));
            slide->setIsExposed(exposed);
        }
    }
}

void Slideable::updateCache()
{
    // Remove excess cached items
    while (m_cachedItems.size() > m_cacheSize) {
        QQuickItem *item = m_cachedItems.takeFirst();
        Slide *slide = qobject_cast<Slide*>(qmlAttachedPropertiesObject<Slide>(item, false));
        if (slide) {
            slide->setInCache(false);
        }
    }

    // Start cache expiry timer
    if (!m_cachedItems.isEmpty() && m_cacheExpiry > 0) {
        m_cacheTimer.start(m_cacheExpiry, this);
    }
}

void Slideable::evictExpiredItems()
{
    QList<QQuickItem*> toRemove;

    for (QQuickItem *item : m_cachedItems) {
        Slide *slide = qobject_cast<Slide*>(qmlAttachedPropertiesObject<Slide>(item, false));
        if (slide && !slide->keepAlive()) {
            toRemove.append(item);
        }
    }

    for (QQuickItem *item : toRemove) {
        m_cachedItems.removeOne(item);
        Slide *slide = qobject_cast<Slide*>(qmlAttachedPropertiesObject<Slide>(item, false));
        if (slide) {
            slide->setInCache(false);
        }
    }
}
