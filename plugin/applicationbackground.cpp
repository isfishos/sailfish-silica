// SPDX-License-Identifier: LGPL-2.1-only

#include "applicationbackground.h"
#include <QQuickWindow>

ApplicationBackground::ApplicationBackground(QObject *parent)
    : QObject(parent)
{
}

void ApplicationBackground::setApplicationWindow(QQuickWindow *w)
{
    if (m_appWindow == w)
        return;
    m_appWindow = w;
    if (m_completed)
        updateWallpaperWindow();
}

void ApplicationBackground::complete()
{
    m_completed = true;
    updateWallpaperWindow();
}

void ApplicationBackground::updateWallpaperWindow()
{
    if (!m_completed || !m_appWindow)
        return;

    // Precedence: wallpaper > image > material/color
    const bool needsWindow = !m_wallpaper.isUndefined() || !m_image.isEmpty() || 
                            !m_filter.isEmpty() || m_color.isValid() || !m_material.isEmpty();

    if (needsWindow && !m_wallpaperWindow) {
        createWallpaperWindow();
    } else if (!needsWindow && m_wallpaperWindow) {
        destroyWallpaperWindow();
    }
}

void ApplicationBackground::createWallpaperWindow()
{
    if (!m_appWindow)
        return;

    // Create wallpaper window behind main window
    m_wallpaperWindow = new QQuickWindow();
    if (m_wallpaperWindow) {
        m_wallpaperWindow->setGeometry(m_appWindow->geometry());
        m_wallpaperWindow->setVisible(m_appWindow->isVisible());
        // TODO: set window flags to stay behind main window
    }
}

void ApplicationBackground::destroyWallpaperWindow()
{
    if (m_wallpaperWindow) {
        m_wallpaperWindow->deleteLater();
        m_wallpaperWindow = nullptr;
    }
}
