// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativecover.h"
#include "declarativecoveractionarea.h"
#include <QQuickWindow>

DeclarativeCover::DeclarativeCover(QQuickItem *parent)
    : QQuickItem(parent)
    , m_status("Inactive")
    , m_size(80, 80) // Default small cover size
{
    // Create the action area as a child
    m_actionArea = new DeclarativeCoverActionArea(this);
    m_actionArea->setParentItem(this);

    // Set initial size
    setImplicitSize(m_size.width(), m_size.height());
}

void DeclarativeCover::setAllowResize(bool allow)
{
    if (m_allowResize != allow) {
        m_allowResize = allow;
        emit allowResizeChanged();
    }
}

void DeclarativeCover::setTransparent(bool transparent)
{
    if (m_transparent != transparent) {
        m_transparent = transparent;
        emit transparentChanged();
    }
}

void DeclarativeCover::setApplicationWindow(QQuickWindow *window)
{
    if (m_applicationWindow != window) {
        m_applicationWindow = window;
        emit applicationWindowChanged();
    }
}

void DeclarativeCover::tryResize(int width, int height, bool allowResize)
{
    if (m_allowResize || allowResize) {
        emit requestResize(width, height);
        // Update transparent state if needed
        if (width > 80 || height > 80) {
            setTransparent(false);
        }
    }
}

void DeclarativeCover::updateStatus()
{
    // Status should only be mutated via DBus/system callbacks
    // For now, keep as "Inactive" until system integration
}

void DeclarativeCover::updateSize()
{
    // Size should reflect current small/large state from system
    // For now, use default small size
    if (m_size != QSize(80, 80)) {
        m_size = QSize(80, 80);
        setImplicitSize(m_size.width(), m_size.height());
        emit sizeChanged();
    }
}
