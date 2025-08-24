// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativewindow.h"
#include "applicationbackground.h"
#include "declarativeorientation.h"
#include "silicascreen.h"
#include <QQuickWindow>
#include <QWindow>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <qpa/qplatformnativeinterface.h>

DeclarativeWindow::DeclarativeWindow(QQuickItem *parent)
    : Silica::Control(parent)
{
    // Create background object
    m_background = new ApplicationBackground(this);

    // Initialize screen rotation based on primary screen orientation
    // For landscape-primary screens, we need 90° rotation to make portrait the "natural" orientation
    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        Qt::ScreenOrientation primaryOrientation = screen->primaryOrientation();
        if (primaryOrientation == Qt::LandscapeOrientation ||
            primaryOrientation == Qt::InvertedLandscapeOrientation ||
            (primaryOrientation == Qt::PrimaryOrientation && screen->size().width() > screen->size().height())) {
            m_screenRotation = 90;  // Landscape-primary needs 90° compensation
        } else {
            m_screenRotation = 0;   // Portrait-primary needs no compensation
        }
    } else {
        m_screenRotation = 0;
    }

    // Initialize device orientation from screen, similar to original
    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        // Get initial device orientation from screen orientation
        m_deviceOrientation = static_cast<int>(screen->orientation());
        if (m_deviceOrientation == 0) {
            // If screen reports no orientation, infer from geometry
            m_deviceOrientation = (screen->size().width() > screen->size().height())
                                 ? DeclarativeOrientation::Landscape
                                 : DeclarativeOrientation::Portrait;
        }

        setSize(screen->size());
        // Connect to screen geometry changes like original implementation
        connect(screen, &QScreen::geometryChanged, this, [this](const QRect &geometry) {
            setSize(geometry.size());
        });

        // Connect to screen orientation changes to update device orientation
        connect(screen, &QScreen::orientationChanged, this, [this](Qt::ScreenOrientation orientation) {
            if (orientation != Qt::PrimaryOrientation) {
                setDeviceOrientation(static_cast<int>(orientation));
            }
        });
    } else {
        // Fallback for no screen
        setSize(QSizeF(540, 960));
        m_deviceOrientation = DeclarativeOrientation::Portrait;
    }

    // Initialize effective orientation based on device orientation and allowed orientations
    updateOrientation();

    // Connect to window changes to emit our windowChanged signal
    connect(this, &QQuickItem::windowChanged, this, &DeclarativeWindow::windowChanged);
    connect(this, &QQuickItem::windowChanged, this, &DeclarativeWindow::onWindowChanged);
}

ApplicationBackground* DeclarativeWindow::background() const
{
    return m_background;
}

qreal DeclarativeWindow::width() const
{
    if (QQuickWindow *w = QQuickItem::window()) {
        return w->width();
    }
    return QQuickItem::width();
}

void DeclarativeWindow::setWidth(qreal width)
{
    if (QQuickWindow *w = QQuickItem::window()) {
        if (w->width() != width) {
            w->setWidth(width);
            emit widthChanged();
        }
    }
}

qreal DeclarativeWindow::height() const
{
    if (QQuickWindow *w = QQuickItem::window()) {
        return w->height();
    }
    return QQuickItem::height();
}

void DeclarativeWindow::setHeight(qreal height)
{
    if (QQuickWindow *w = QQuickItem::window()) {
        if (w->height() != height) {
            w->setHeight(height);
            emit heightChanged();
        }
    }
}

void DeclarativeWindow::setOrientation(int orientation)
{
    if (m_orientation != orientation) {
        m_orientation = orientation;

        // Report content orientation to window system
        reportContentOrientation(window(), orientation);

        emit orientationChanged();

        // Update window size when orientation changes
        updateWindowSize();
    }
}

void DeclarativeWindow::setExplicitOrientation(int orientation)
{
    m_explicitOrientation = true;
    setOrientation(orientation);
}

void DeclarativeWindow::resetOrientation()
{
    m_explicitOrientation = false;
    updateOrientation();
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

int DeclarativeWindow::screenRotation() const
{
    // Return the configured screen rotation value
    // In original implementation, this could be configured via MGConf
    // For desktop/non-Sailfish use, default to 0 (no rotation)
    return m_screenRotation;
}

void DeclarativeWindow::_processPendingDeletions()
{
    // Process any pending deletions
    QCoreApplication::processEvents();
}

int DeclarativeWindow::_selectOrientation(int allowed, int suggested) const
{
    // Handle the case where suggested == -1 by using device orientation
    int effectiveSuggested = (suggested == -1) ? m_deviceOrientation : suggested;

    // Delegate to selectOrientation with the effective suggested orientation
    return selectOrientation(allowed, effectiveSuggested);
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

    // Ensure orientation is fully resolved after completion
    updateOrientation();
}

void DeclarativeWindow::updateOrientation()
{
    // Follow original implementation logic for automatic orientation selection
    // Only update if orientation is not explicitly set
    if (!m_explicitOrientation) {
        int selectedOrientation = selectOrientation(m_allowedOrientations, m_deviceOrientation);
        setOrientation(selectedOrientation);
    }

    // Auto-set page orientation to match window orientation if not explicitly set by a page
    // This matches the behavior seen in the original library
    if (m_pageOrientation == -1) {
        setPageOrientation(m_orientation);
    }
}

int DeclarativeWindow::selectOrientation(int allowedOrientations, int deviceOrientation) const
{
    int selectedOrientation = DeclarativeOrientation::None;

    // If the current device orientation is allowed, select that
    if (allowedOrientations & deviceOrientation) {
        selectedOrientation = deviceOrientation;
    } else if (m_orientation & allowedOrientations) {
        // If current orientation is still allowed, keep it
        return m_orientation;
    } else {
        // Choose the first permissible orientation from the allowed set (in preference order)
        if (allowedOrientations & DeclarativeOrientation::Portrait) {
            selectedOrientation = DeclarativeOrientation::Portrait;
        } else if (allowedOrientations & DeclarativeOrientation::Landscape) {
            selectedOrientation = DeclarativeOrientation::Landscape;
        } else if (allowedOrientations & DeclarativeOrientation::LandscapeInverted) {
            selectedOrientation = DeclarativeOrientation::LandscapeInverted;
        } else if (allowedOrientations & DeclarativeOrientation::PortraitInverted) {
            selectedOrientation = DeclarativeOrientation::PortraitInverted;
        }
    }

    return selectedOrientation;
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

void DeclarativeWindow::updateWindowProperties()
{
    QWindow * const window = QQuickItem::window();
    if (!window) return;

    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    if (!native) return;

    // Only set properties for Wayland platform
    if (QGuiApplication::platformName().startsWith("wayland")) {
        QPlatformWindow *handle = window->handle();
        if (handle) {
            native->setWindowProperty(handle, QStringLiteral("BACKGROUND_VISIBLE"), m_backgroundVisible);
            native->setWindowProperty(handle, QStringLiteral("WINID"), window->winId());
            native->setWindowProperty(handle, QStringLiteral("SAILFISH_HAVE_COVER"), m_haveCoverHint);
            native->setWindowProperty(handle, QStringLiteral("WINDOW_OPACITY"), m_windowOpacity);

            if (m_backgroundRect.isValid()) {
                native->setWindowProperty(handle, QStringLiteral("BACKGROUND_RECT"), m_backgroundRect);
            }
        }
    }
}

void DeclarativeWindow::onWindowChanged()
{
    static QQuickWindow *previousWindow = nullptr;

    if (previousWindow) {
        // Disconnect from previous window
        disconnect(previousWindow, &QQuickWindow::widthChanged, this, &DeclarativeWindow::widthChanged);
        disconnect(previousWindow, &QQuickWindow::heightChanged, this, &DeclarativeWindow::heightChanged);
    }

    if (QQuickWindow *w = QQuickItem::window()) {
        // Connect to new window size changes
        connect(w, &QQuickWindow::widthChanged, this, &DeclarativeWindow::widthChanged);
        connect(w, &QQuickWindow::heightChanged, this, &DeclarativeWindow::heightChanged);
        previousWindow = w;

        // Size the window based on screen dimensions and orientation
        updateWindowSize();

        // Set Wayland window properties for SailfishOS compositor
        updateWindowProperties();

        // Report current content orientation to window system
        // This is critical for proper coordinate transformation
        if (m_orientation != DeclarativeOrientation::None) {
            reportContentOrientation(w, m_orientation);
        }
    }
}

void DeclarativeWindow::updateWindowSize()
{
    // Original implementation doesn't change the window size
    // TODO: Implement window resizing logic when running on desktop
}

void DeclarativeWindow::reportContentOrientation(QWindow *window, int orientation)
{
    if (!window) {
        return;
    }

    // Convert DeclarativeOrientation to Qt::ScreenOrientation
    Qt::ScreenOrientation qtOrientation;
    switch(orientation) {
    case DeclarativeOrientation::Portrait:
        qtOrientation = Qt::PortraitOrientation;
        break;
    case DeclarativeOrientation::Landscape:
        qtOrientation = Qt::LandscapeOrientation;
        break;
    case DeclarativeOrientation::PortraitInverted:
        qtOrientation = Qt::InvertedPortraitOrientation;
        break;
    case DeclarativeOrientation::LandscapeInverted:
        qtOrientation = Qt::InvertedLandscapeOrientation;
        break;
    default:
        qtOrientation = Qt::PortraitOrientation;
        break;
    }

    // Report content orientation to window system - crucial for proper coordinate transformation
    window->reportContentOrientationChange(qtOrientation);
}

QWindow* DeclarativeWindow::window() const
{
    return QQuickItem::window();
}
