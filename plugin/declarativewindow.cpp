// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativewindow.h"
#include "applicationbackground.h"
#include <QQuickWindow>
#include <QWindow>
#include <QCoreApplication>

DeclarativeWindow::DeclarativeWindow(QQuickItem *parent)
    : Silica::Control(parent)
{
    // Create background object
    m_background = new ApplicationBackground(this);
    // Default device orientation to portrait to avoid undefined in QML
    m_deviceOrientation = 1; // Orientation::Portrait
    // Initialize effective orientation based on defaults
    updateOrientation();

    // Connect to window changes to emit our windowChanged signal
    connect(this, &QQuickItem::windowChanged, this, &DeclarativeWindow::windowChanged);
}

ApplicationBackground* DeclarativeWindow::background() const
{
    return m_background;
}

void DeclarativeWindow::setOrientation(int orientation)
{
    if (m_orientation != orientation) {
        m_orientation = orientation;
        emit orientationChanged();
    }
}

void DeclarativeWindow::setPageOrientation(int orientation)
{
    if (m_pageOrientation != orientation) {
        m_pageOrientation = orientation;
        emit pageOrientationChanged();
    }
}

void DeclarativeWindow::resetPageOrientation()
{
    setPageOrientation(-1);
}

void DeclarativeWindow::setDeviceOrientation(int orientation)
{
    if (m_deviceOrientation != orientation) {
        m_deviceOrientation = orientation;
        emit deviceOrientationChanged();
        updateOrientation();
    }
}

void DeclarativeWindow::setAllowedOrientations(int orientations)
{
    if (m_allowedOrientations != orientations) {
        m_allowedOrientations = orientations;
        emit allowedOrientationsChanged();
        updateOrientation();
    }
}

void DeclarativeWindow::setBackgroundVisible(bool visible)
{
    if (m_backgroundVisible != visible) {
        m_backgroundVisible = visible;
        emit backgroundVisibleChanged();
    }
}

void DeclarativeWindow::setOpaque(bool opaque)
{
    if (m_opaque != opaque) {
        m_opaque = opaque;
        emit opaqueChanged();
    }
}

void DeclarativeWindow::resetOpaque()
{
    setOpaque(true);
}

void DeclarativeWindow::setPersistentOpenGLContext(bool persistent)
{
    if (m_persistentOpenGLContext != persistent) {
        m_persistentOpenGLContext = persistent;
        updateWindowFlags();
        emit persistentOpenGLContextChanged();
    }
}

void DeclarativeWindow::setPersistentSceneGraph(bool persistent)
{
    if (m_persistentSceneGraph != persistent) {
        m_persistentSceneGraph = persistent;
        updateWindowFlags();
        emit persistentSceneGraphChanged();
    }
}

void DeclarativeWindow::setHaveCoverHint(bool hint)
{
    if (m_haveCoverHint != hint) {
        m_haveCoverHint = hint;
        emit haveCoverHintChanged();
    }
}

void DeclarativeWindow::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged();
    }
}

void DeclarativeWindow::setCoverIsPrimaryWindow(bool primary)
{
    if (m_coverIsPrimaryWindow != primary) {
        m_coverIsPrimaryWindow = primary;
        emit coverIsPrimaryWindowChanged();
    }
}

void DeclarativeWindow::setCoverVisible(bool visible)
{
    if (m_coverVisible != visible) {
        m_coverVisible = visible;
        emit coverVisibleChanged();
    }
}

void DeclarativeWindow::resetCoverVisible()
{
    setCoverVisible(false);
}

void DeclarativeWindow::setProxyWindow(QQuickWindow *window)
{
    if (m_proxyWindow != window) {
        m_proxyWindow = window;
        emit proxyWindowChanged();
    }
}

void DeclarativeWindow::setMainWindow(QWindow *window)
{
    if (m_mainWindow != window) {
        m_mainWindow = window;
        emit mainWindowChanged();
    }
}

void DeclarativeWindow::setWindowOpacity(qreal opacity)
{
    if (m_windowOpacity != opacity) {
        m_windowOpacity = opacity;
        emit windowOpacityChanged();
    }
}

void DeclarativeWindow::activate()
{
    if (QQuickWindow *window = QQuickItem::window()) {
        window->show();
        window->requestActivate();
    }
}

void DeclarativeWindow::deactivate()
{
    if (QQuickWindow *window = QQuickItem::window()) {
        window->hide();
    }
}

void DeclarativeWindow::_processPendingDeletions()
{
    // Process any pending deletions
    QCoreApplication::processEvents();
}

int DeclarativeWindow::_selectOrientation(int allowed, int suggested) const
{
    int bestOrientation = 0;
    if (suggested >= 0 && (allowed & suggested)) {
        bestOrientation = suggested;
    } else {
        // Orientation enum mask order: Portrait(1), Landscape(2), PortraitInverted(4), LandscapeInverted(8)
        const int candidates[4] = { 1, 2, 4, 8 };
        for (int i = 0; i < 4; ++i) {
            if (allowed & candidates[i]) {
                bestOrientation = candidates[i];
                break;
            }
        }
    }
    return bestOrientation;
}

void DeclarativeWindow::_setCover(QObject *cover)
{
    // TODO: Implement cover management
    Q_UNUSED(cover)
}

void DeclarativeWindow::_updateCoverVisibility()
{
    // Update cover visibility based on current state
    if (m_haveCoverHint && m_coverVisible) {
        // Show cover
    } else {
        // Hide cover
    }
}

void DeclarativeWindow::componentComplete()
{
    QQuickItem::componentComplete();

    // Connect to screen geometry changes
    if (QQuickWindow *window = QQuickItem::window()) {
        connect(window, &QQuickWindow::widthChanged, this, [this, window]() {
            m_backgroundRect = QRectF(0, 0, window->width(), window->height());
            emit backgroundRectChanged();
        });
        connect(window, &QQuickWindow::heightChanged, this, [this, window]() {
            m_backgroundRect = QRectF(0, 0, window->width(), window->height());
            emit backgroundRectChanged();
        });
    }

    // Connect to app aboutToQuit
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, [this]() {
        deactivate();
    });

    // Initialize background rect
    if (QQuickWindow *window = QQuickItem::window()) {
        m_backgroundRect = QRectF(0, 0, window->width(), window->height());
    }
}

void DeclarativeWindow::updateOrientation()
{
    // Update orientation based on device orientation and allowed orientations
    int newOrientation = m_deviceOrientation & m_allowedOrientations;
    if (newOrientation == 0) {
        // If no orientation matches, use the first allowed one
        for (int i = 0; i < 4; ++i) {
            if (m_allowedOrientations & (1 << i)) {
                newOrientation = 1 << i;
                break;
            }
        }
    }

    setOrientation(newOrientation);
}

void DeclarativeWindow::updateWindowFlags()
{
    if (QQuickWindow *window = QQuickItem::window()) {
        // Set window flags based on persistence hints
        Qt::WindowFlags flags = window->flags();

        if (m_persistentOpenGLContext) {
            flags |= Qt::WindowStaysOnTopHint;
        }

        if (m_persistentSceneGraph) {
            // Keep scene graph persistent
        }

        window->setFlags(flags);
    }
}

QWindow* DeclarativeWindow::window() const
{
    return QQuickItem::window();
}
