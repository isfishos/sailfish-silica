// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_WAYLANDBLURMANAGER_H
#define SAILFISH_SILICA_PLUGIN_WAYLANDBLURMANAGER_H

#include <QObject>

class QQuickWindow;

#ifdef HAVE_KWAYLAND

/**
 * @brief Manages KWayland blur behind effects for windows
 *
 * This class encapsulates all KWayland-specific blur functionality,
 * keeping the main DeclarativeWindow class clean and focused.
 */
class WaylandBlurManager : public QObject
{
    Q_OBJECT

public:
    explicit WaylandBlurManager(QObject *parent = nullptr);
    ~WaylandBlurManager() override;

    /**
     * @brief Setup blur behind effect for the given window
     * @param window The QQuickWindow to apply blur to
     * @return true if setup was initiated successfully, false otherwise
     */
    bool setupBlurBehind(QQuickWindow *window);

    /**
     * @brief Check if blur setup has been completed
     * @return true if blur is already set up
     */
    bool isBlurSetup() const { return m_blurSetup; }

    /**
     * @brief Reset the blur manager state (useful when window changes)
     */
    void reset();

private slots:
    void onBlurAnnounced(quint32 name, quint32 version);

private:
    QObject *m_registry = nullptr;
    QQuickWindow *m_window = nullptr;
    bool m_blurSetup = false;
};

#else

// Stub implementation when KWayland is not available
class WaylandBlurManager : public QObject
{
    Q_OBJECT

public:
    explicit WaylandBlurManager(QObject *parent = nullptr) : QObject(parent) {}
    bool setupBlurBehind(QQuickWindow *) { return false; }
    bool isBlurSetup() const { return false; }
    void reset() {}
};

#endif // HAVE_KWAYLAND

#endif // SAILFISH_SILICA_PLUGIN_WAYLANDBLURMANAGER_H
