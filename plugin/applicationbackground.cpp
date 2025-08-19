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

void ApplicationBackground::setWallpaper(const QJSValue &v)
{
    if ((m_wallpaper.isUndefined() && v.isUndefined()) || m_wallpaper.strictlyEquals(v)) {
        return;
    }
    m_wallpaper = v;
    emit wallpaperChanged();
    emit patternChanged();
    updateWallpaperWindow();
}

void ApplicationBackground::setImage(const QUrl &u)
{
    if (m_image == u)
        return;
    m_image = u;
    emit imageChanged();
    emit patternChanged();
    updateWallpaperWindow();
}

void ApplicationBackground::setFilter(const QString &f)
{
    if (m_filter == f)
        return;
    m_filter = f;
    m_explicitFilter = true;
    emit filterChanged();
    emit patternChanged();
    updateWallpaperWindow();
}

void ApplicationBackground::setColor(const QColor &c)
{
    if (m_color == c)
        return;
    m_color = c;
    emit colorChanged();
    emit patternChanged();
}

void ApplicationBackground::setMaterial(const QString &m)
{
    if (m_material == m)
        return;
    m_material = m;
    emit materialChanged();
    emit patternChanged();
}

void ApplicationBackground::setWindowComponent(QQmlComponent *c)
{
    if (m_windowComponent == c)
        return;
    m_windowComponent = c;
    emit windowComponentChanged();
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
