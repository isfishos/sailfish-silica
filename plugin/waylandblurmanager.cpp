// SPDX-License-Identifier: LGPL-2.1-only

#include "waylandblurmanager.h"

#ifdef HAVE_KWAYLAND

#include <QQuickWindow>
#include <QDebug>
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

#include <KWayland/Client/connection_thread.h>
#include <KWayland/Client/registry.h>
#include <KWayland/Client/blur.h>
#include <KWayland/Client/surface.h>

using namespace KWayland::Client;

WaylandBlurManager::WaylandBlurManager(QObject *parent)
    : QObject(parent)
{
}

WaylandBlurManager::~WaylandBlurManager()
{
    reset();
}

bool WaylandBlurManager::setupBlurBehind(QQuickWindow *window)
{
    if (!window || m_registry || m_blurSetup) {
        return false; // Already setup or no window
    }

    m_window = window;

    // Get the native Wayland surface
    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    if (!native) {
        qDebug() << "No platform native interface available";
        return false;
    }

    auto connection = ConnectionThread::fromApplication();
    if (!connection) {
        qDebug() << "No Wayland connection available";
        return false;
    }

    auto registry = new Registry(this);
    registry->create(connection);
    registry->setup();
    m_registry = registry;

    // Connect to blur manager announcements
    connect(registry, &Registry::blurAnnounced, this, &WaylandBlurManager::onBlurAnnounced);

    return true;
}

void WaylandBlurManager::reset()
{
    if (m_registry) {
        m_registry->deleteLater();
        m_registry = nullptr;
    }
    m_window = nullptr;
    m_blurSetup = false;
}

void WaylandBlurManager::onBlurAnnounced(quint32 name, quint32 version)
{
    auto *registry = qobject_cast<KWayland::Client::Registry*>(m_registry);
    if (!registry || !m_window) {
        return;
    }

    auto blurManager = registry->createBlurManager(name, version, this);
    if (blurManager) {
        // Create a KWayland surface wrapper
        auto surface = Surface::fromWindow(m_window);

        // Create blur for the entire window
        auto blur = blurManager->createBlur(surface, this);
        if (blur) {
            // Blur entire surface (no region specified)
            blur->commit();
            qDebug() << "Blur behind effect enabled for window";

            // Mark blur as setup and disconnect to avoid duplicates
            m_blurSetup = true;
            disconnect(registry, &Registry::blurAnnounced, this, &WaylandBlurManager::onBlurAnnounced);
        }
    }
}

#endif // HAVE_KWAYLAND
