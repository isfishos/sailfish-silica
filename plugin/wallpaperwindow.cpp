// SPDX-License-Identifier: LGPL-2.1-only

#include "wallpaperwindow.h"
#include <QEvent>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QShowEvent>
#include <QHideEvent>

WallpaperWindow::WallpaperWindow(QQuickWindow *parent)
    : QQuickWindow()
    , m_windowVisible(true)
    , m_parentWindow(nullptr)
{
    if (parent) {
        setParentWindow(parent);
    }
}

void WallpaperWindow::setWindowVisible(bool visible)
{
    if (m_windowVisible != visible) {
        m_windowVisible = visible;
        if (m_parentWindow) {
            setVisible(m_parentWindow->isVisible() && m_windowVisible);
        }
        emit windowVisibleChanged();
    }
}

void WallpaperWindow::setParentWindow(QQuickWindow *window)
{
    if (m_parentWindow != window) {
        if (m_parentWindow) {
            m_parentWindow->removeEventFilter(this);
        }

        m_parentWindow = window;

        if (m_parentWindow) {
            m_parentWindow->installEventFilter(this);
            syncWithParent();
        }

        emit parentWindowChanged();
    }
}

void WallpaperWindow::setWindowWidth(qreal width)
{
    if (!qFuzzyCompare(this->width(), width)) {
        resize(static_cast<int>(width), height());
        emit windowWidthChanged();
    }
}

void WallpaperWindow::setWindowHeight(qreal height)
{
    if (!qFuzzyCompare(this->height(), height)) {
        resize(width(), static_cast<int>(height));
        emit windowHeightChanged();
    }
}

bool WallpaperWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        updateGeometry();
        return true;
    }
    return QQuickWindow::event(event);
}

bool WallpaperWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_parentWindow) {
        switch (event->type()) {
        case QEvent::Resize:
        case QEvent::Move:
        case QEvent::Show:
        case QEvent::Hide:
            syncWithParent();
            break;
        default:
            break;
        }
    }
    return QQuickWindow::eventFilter(watched, event);
}

void WallpaperWindow::updateGeometry()
{
    if (m_parentWindow) {
        syncWithParent();
    }
}

void WallpaperWindow::syncWithParent()
{
    if (!m_parentWindow) {
        return;
    }

    // Sync geometry
    QRect parentGeometry = m_parentWindow->geometry();
    if (geometry() != parentGeometry) {
        setGeometry(parentGeometry);
    }

    // Sync visibility
    bool shouldBeVisible = m_parentWindow->isVisible() && m_windowVisible;
    if (isVisible() != shouldBeVisible) {
        setVisible(shouldBeVisible);
    }
}
