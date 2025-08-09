// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativevisibilitycull.h"
#include <QQuickWindow>
#include <QQuickItem>

DeclarativeVisibilityCull::DeclarativeVisibilityCull(QQuickItem *parent)
    : QQuickItem(parent)
{
    connect(&m_syncTimer, &QTimer::timeout, this, &DeclarativeVisibilityCull::sync);
    m_syncTimer.setSingleShot(true);
    m_syncTimer.setInterval(16); // ~60fps
}

void DeclarativeVisibilityCull::setTarget(QQuickItem *target)
{
    if (m_target != target) {
        restoreHiddenItems();
        m_target = target;
        emit targetChanged();

        if (m_enabled && m_target) {
            updateVisibility();
        }
    }
}

void DeclarativeVisibilityCull::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        emit enabledChanged();

        if (!m_enabled) {
            restoreHiddenItems();
        } else if (m_target) {
            updateVisibility();
        }
    }
}

void DeclarativeVisibilityCull::itemChange(ItemChange change, const ItemChangeData &value)
{
    if (change == ItemSceneChange && m_enabled && m_target) {
        updateVisibility();
    }

    QQuickItem::itemChange(change, value);
}

void DeclarativeVisibilityCull::sync()
{
    if (m_enabled && m_target) {
        updateVisibility();
    }
}

void DeclarativeVisibilityCull::updateVisibility()
{
    if (!m_target || !m_enabled) {
        return;
    }

    // Schedule sync for next frame
    if (!m_syncTimer.isActive()) {
        m_syncTimer.start();
    }

    // Get the visible rect of the viewport
    QRectF viewportRect;
    if (QQuickWindow *window = m_target->window()) {
        viewportRect = QRectF(0, 0, window->width(), window->height());
    } else {
        viewportRect = m_target->boundingRect();
    }

    // Check each child of the target
    QList<QQuickItem*> children = m_target->childItems();
    QSet<QQuickItem*> currentlyHidden;

    for (QQuickItem *child : children) {
        if (!child || !child->isVisible()) {
            continue;
        }

        // Map child's bounding rect to viewport coordinates
        QRectF childRect = child->mapRectToScene(child->boundingRect());

        // Check if child intersects with viewport
        if (!viewportRect.intersects(childRect)) {
            // Hide the child
            child->setVisible(false);
            currentlyHidden.insert(child);
        } else {
            // Show the child if it was previously hidden by us
            if (m_hiddenItems.contains(child)) {
                child->setVisible(true);
            }
        }
    }

    // Update our hidden items set
    m_hiddenItems = currentlyHidden;
}

void DeclarativeVisibilityCull::restoreHiddenItems()
{
    // Restore all items that we hid
    for (QQuickItem *item : m_hiddenItems) {
        if (item) {
            item->setVisible(true);
        }
    }
    m_hiddenItems.clear();
}
