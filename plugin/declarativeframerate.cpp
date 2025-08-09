// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativeframerate.h"
#include <QElapsedTimer>
#include <QStandardPaths>
#include <QSettings>

DeclarativeFrameRate::DeclarativeFrameRate(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
{
    // Check if framerate display is enabled via configuration
    QSettings settings;
    m_enabled = settings.value("debug/framerate", false).toBool();

    connect(m_timer, &QTimer::timeout, this, &DeclarativeFrameRate::updateFrameTime);
    m_timer->start(1000); // Update every second
}

DeclarativeFrameRate::~DeclarativeFrameRate()
{
    if (m_window) {
        disconnect(m_window, nullptr, this, nullptr);
    }
}

void DeclarativeFrameRate::setWindow(QQuickWindow *window)
{
    if (m_window == window) {
        return;
    }

    if (m_window) {
        disconnect(m_window, nullptr, this, nullptr);
    }

    m_window = window;

    if (m_window && m_enabled) {
        connect(m_window, &QQuickWindow::beforeRendering, this, &DeclarativeFrameRate::onBeforeRendering);
        connect(m_window, &QQuickWindow::afterRendering, this, &DeclarativeFrameRate::onAfterRendering);
        connect(m_window, &QQuickWindow::beforeSynchronizing, this, &DeclarativeFrameRate::onBeforeSynchronizing);
        connect(m_window, &QQuickWindow::sceneGraphInvalidated, this, &DeclarativeFrameRate::onSceneGraphInvalidated);
    }
}

void DeclarativeFrameRate::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;

        if (m_window) {
            if (enabled) {
                connect(m_window, &QQuickWindow::beforeRendering, this, &DeclarativeFrameRate::onBeforeRendering);
                connect(m_window, &QQuickWindow::afterRendering, this, &DeclarativeFrameRate::onAfterRendering);
                connect(m_window, &QQuickWindow::beforeSynchronizing, this, &DeclarativeFrameRate::onBeforeSynchronizing);
                connect(m_window, &QQuickWindow::sceneGraphInvalidated, this, &DeclarativeFrameRate::onSceneGraphInvalidated);
            } else {
                disconnect(m_window, nullptr, this, nullptr);
            }
        }

        // Save setting
        QSettings settings;
        settings.setValue("debug/framerate", enabled);
    }
}

void DeclarativeFrameRate::onBeforeRendering()
{
    if (!m_enabled) {
        return;
    }

    static QElapsedTimer timer;
    if (!timer.isValid()) {
        timer.start();
    }

    m_lastFrameTime = timer.nsecsElapsed();
}

void DeclarativeFrameRate::onAfterRendering()
{
    if (!m_enabled) {
        return;
    }

    updateFrameTime();
    renderHUD();
}

void DeclarativeFrameRate::onBeforeSynchronizing()
{
    // Could be used for additional timing if needed
}

void DeclarativeFrameRate::onSceneGraphInvalidated()
{
    // Clean up GL resources if needed
    m_frameTimes.clear();
}

void DeclarativeFrameRate::updateFrameTime()
{
    if (!m_enabled || !m_window) {
        return;
    }

    static QElapsedTimer timer;
    if (!timer.isValid()) {
        timer.start();
        return;
    }

    qint64 currentTime = timer.nsecsElapsed();
    qint64 frameDuration = currentTime - m_lastFrameTime;

    m_frameTimes.append(frameDuration);

    // Keep only the last FRAME_BUFFER_SIZE frames
    if (m_frameTimes.size() > FRAME_BUFFER_SIZE) {
        m_frameTimes.removeFirst();
    }

    m_lastFrameTime = currentTime;
}

void DeclarativeFrameRate::renderHUD()
{
    if (!m_enabled || !m_window || m_frameTimes.isEmpty()) {
        return;
    }

    // Calculate average frame time and FPS
    qint64 totalTime = 0;
    for (qint64 time : m_frameTimes) {
        totalTime += time;
    }

    qint64 avgFrameTime = totalTime / m_frameTimes.size();
    double fps = 1000000000.0 / avgFrameTime; // Convert nanoseconds to FPS

    // TODO: Render HUD using QSG
    // This would involve creating a custom QSG node with simple geometry
    // (bars/lines) to display the framerate information

    // For now, just log the FPS
    qDebug() << "FPS:" << fps << "Avg frame time:" << avgFrameTime / 1000000.0 << "ms";
}
