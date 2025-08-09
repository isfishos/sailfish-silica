// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativecoverwindow.h"
#include <QResizeEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QGuiApplication>

DeclarativeCoverWindow::DeclarativeCoverWindow(QWindow *parent)
    : QQuickWindow(parent)
    , m_coverSize(80, 80)
{
    // Set window flags for cover window
    setFlags(Qt::Window | Qt::FramelessWindowHint);

    // Find main application window
    for (QWindow *window : QGuiApplication::topLevelWindows()) {
        if (QQuickWindow *quickWindow = qobject_cast<QQuickWindow*>(window)) {
            if (quickWindow != this) {
                m_mainWindow = quickWindow;
                break;
            }
        }
    }

    setCoverSize(m_coverSize);
}

void DeclarativeCoverWindow::setCoverSize(const QSize &size)
{
    if (m_coverSize != size) {
        m_coverSize = size;
        updateGeometry();
        emit coverSizeChanged();
    }
}

void DeclarativeCoverWindow::resizeEvent(QResizeEvent *event)
{
    QQuickWindow::resizeEvent(event);
    syncWithMainWindow();
}

void DeclarativeCoverWindow::showEvent(QShowEvent *event)
{
    QQuickWindow::showEvent(event);
    syncWithMainWindow();
}

void DeclarativeCoverWindow::hideEvent(QHideEvent *event)
{
    QQuickWindow::hideEvent(event);
    // Clean up when hidden
}

void DeclarativeCoverWindow::updateGeometry()
{
    if (m_mainWindow) {
        // Position cover window relative to main window
        QRect mainGeometry = m_mainWindow->geometry();
        QPoint coverPos = mainGeometry.topLeft() + QPoint(10, 10); // Offset from main window
        setGeometry(QRect(coverPos, m_coverSize));
    }
}

void DeclarativeCoverWindow::syncWithMainWindow()
{
    if (m_mainWindow && isVisible()) {
        // Keep synchronized with main window visibility and geometry
        if (!m_mainWindow->isVisible()) {
            hide();
        }
    }
}
