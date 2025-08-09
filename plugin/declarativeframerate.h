// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEFRAMERATE_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEFRAMERATE_H

#include <QObject>
#include <QQuickWindow>
#include <QTimer>

class DeclarativeFrameRate : public QObject
{
    Q_OBJECT

public:
    explicit DeclarativeFrameRate(QObject *parent = nullptr);
    ~DeclarativeFrameRate();

    void setWindow(QQuickWindow *window);
    void setEnabled(bool enabled);

private slots:
    void onBeforeRendering();
    void onAfterRendering();
    void onBeforeSynchronizing();
    void onSceneGraphInvalidated();

private:
    void updateFrameTime();
    void renderHUD();

    QQuickWindow *m_window = nullptr;
    QTimer *m_timer = nullptr;
    QList<qint64> m_frameTimes;
    qint64 m_lastFrameTime = 0;
    bool m_enabled = false;
    static const int FRAME_BUFFER_SIZE = 120;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEFRAMERATE_H
