// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEWINDOW_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEWINDOW_H

#include <QQuickItem>
#include <QQuickWindow>
#include <QWindow>
#include <QColor>
#include <QRectF>

class ApplicationBackground;

class DeclarativeWindow : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(int _pageOrientation READ pageOrientation WRITE setPageOrientation RESET resetPageOrientation NOTIFY pageOrientationChanged)
    Q_PROPERTY(int deviceOrientation READ deviceOrientation WRITE setDeviceOrientation NOTIFY deviceOrientationChanged)
    Q_PROPERTY(int defaultAllowedOrientations READ defaultAllowedOrientations CONSTANT)
    Q_PROPERTY(int _allowedOrientations READ allowedOrientations WRITE setAllowedOrientations NOTIFY allowedOrientationsChanged)
    Q_PROPERTY(int screenRotation READ screenRotation CONSTANT)
    Q_PROPERTY(QObject* background READ background CONSTANT)
    Q_PROPERTY(bool _backgroundVisible READ backgroundVisible WRITE setBackgroundVisible NOTIFY backgroundVisibleChanged)
    Q_PROPERTY(bool _opaque READ opaque WRITE setOpaque RESET resetOpaque NOTIFY opaqueChanged)
    Q_PROPERTY(bool _persistentOpenGLContext READ persistentOpenGLContext WRITE setPersistentOpenGLContext NOTIFY persistentOpenGLContextChanged)
    Q_PROPERTY(bool _persistentSceneGraph READ persistentSceneGraph WRITE setPersistentSceneGraph NOTIFY persistentSceneGraphChanged)
    Q_PROPERTY(bool _haveCoverHint READ haveCoverHint WRITE setHaveCoverHint NOTIFY haveCoverHintChanged)
    Q_PROPERTY(QColor _backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(bool _coverIsPrimaryWindow READ coverIsPrimaryWindow WRITE setCoverIsPrimaryWindow NOTIFY coverIsPrimaryWindowChanged)
    Q_PROPERTY(bool _coverVisible READ coverVisible WRITE setCoverVisible RESET resetCoverVisible NOTIFY coverVisibleChanged)
    Q_PROPERTY(QQuickWindow* _proxyWindow READ proxyWindow WRITE setProxyWindow NOTIFY proxyWindowChanged)
    Q_PROPERTY(QWindow* _mainWindow READ mainWindow WRITE setMainWindow NOTIFY mainWindowChanged)
    Q_PROPERTY(qreal _windowOpacity READ windowOpacity WRITE setWindowOpacity NOTIFY windowOpacityChanged)
    Q_PROPERTY(QRectF _backgroundRect READ backgroundRect NOTIFY backgroundRectChanged)

public:
    explicit DeclarativeWindow(QQuickItem *parent = nullptr);

    int orientation() const { return m_orientation; }
    void setOrientation(int orientation);
    int pageOrientation() const { return m_pageOrientation; }
    void setPageOrientation(int orientation);
    void resetPageOrientation();
    int deviceOrientation() const { return m_deviceOrientation; }
    void setDeviceOrientation(int orientation);
    int defaultAllowedOrientations() const { return 0x0F; } // All orientations
    int allowedOrientations() const { return m_allowedOrientations; }
    void setAllowedOrientations(int orientations);
    int screenRotation() const { return 0; } // TODO: Implement actual screen rotation
    QObject* background() const;
    bool backgroundVisible() const { return m_backgroundVisible; }
    void setBackgroundVisible(bool visible);
    bool opaque() const { return m_opaque; }
    void setOpaque(bool opaque);
    void resetOpaque();
    bool persistentOpenGLContext() const { return m_persistentOpenGLContext; }
    void setPersistentOpenGLContext(bool persistent);
    bool persistentSceneGraph() const { return m_persistentSceneGraph; }
    void setPersistentSceneGraph(bool persistent);
    bool haveCoverHint() const { return m_haveCoverHint; }
    void setHaveCoverHint(bool hint);
    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor &color);
    bool coverIsPrimaryWindow() const { return m_coverIsPrimaryWindow; }
    void setCoverIsPrimaryWindow(bool primary);
    bool coverVisible() const { return m_coverVisible; }
    void setCoverVisible(bool visible);
    void resetCoverVisible();
    QQuickWindow* proxyWindow() const { return m_proxyWindow; }
    void setProxyWindow(QQuickWindow *window);
    QWindow* mainWindow() const { return m_mainWindow; }
    void setMainWindow(QWindow *window);
    qreal windowOpacity() const { return m_windowOpacity; }
    void setWindowOpacity(qreal opacity);
    QRectF backgroundRect() const { return m_backgroundRect; }

    Q_INVOKABLE void activate();
    Q_INVOKABLE void deactivate();
    Q_INVOKABLE void _processPendingDeletions();
    Q_INVOKABLE void _selectOrientation(int allowed, int suggested = -1);
    Q_INVOKABLE void _setCover(QObject *cover);
    Q_INVOKABLE void _updateCoverVisibility();

Q_SIGNALS:
    void orientationChanged();
    void pageOrientationChanged();
    void deviceOrientationChanged();
    void allowedOrientationsChanged();
    void screenRotationChanged();
    void backgroundColorChanged();
    void backgroundVisibleChanged();
    void opaqueChanged();
    void persistentOpenGLContextChanged();
    void persistentSceneGraphChanged();
    void haveCoverHintChanged();
    void coverIsPrimaryWindowChanged();
    void coverVisibleChanged();
    void proxyWindowChanged();
    void mainWindowChanged();
    void windowOpacityChanged();
    void backgroundRectChanged();

protected:
    void componentComplete() override;

private:
    void updateOrientation();
    void updateWindowFlags();

    int m_orientation = 0;
    int m_pageOrientation = -1;
    int m_deviceOrientation = 0;
    int m_allowedOrientations = 0x0F;
    bool m_backgroundVisible = true;
    bool m_opaque = true;
    bool m_persistentOpenGLContext = false;
    bool m_persistentSceneGraph = false;
    bool m_haveCoverHint = false;
    QColor m_backgroundColor = Qt::black;
    bool m_coverIsPrimaryWindow = false;
    bool m_coverVisible = false;
    QQuickWindow *m_proxyWindow = nullptr;
    QWindow *m_mainWindow = nullptr;
    qreal m_windowOpacity = 1.0;
    QRectF m_backgroundRect;
    ApplicationBackground *m_background = nullptr;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEWINDOW_H
