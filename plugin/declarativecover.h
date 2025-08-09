// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECOVER_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECOVER_H

#include <QObject>
#include <QQuickItem>
#include <QSize>
#include <QJSValue>

class DeclarativeCoverActionArea;

class DeclarativeCover : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool allowResize READ allowResize WRITE setAllowResize NOTIFY allowResizeChanged)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
    Q_PROPERTY(bool transparent READ transparent WRITE setTransparent NOTIFY transparentChanged)
    Q_PROPERTY(DeclarativeCoverActionArea* coverActionArea READ coverActionArea CONSTANT)
    Q_PROPERTY(QQuickWindow* applicationWindow READ applicationWindow WRITE setApplicationWindow NOTIFY applicationWindowChanged)

public:
    enum Status {
        Active,
        Inactive,
        Updating
    };
    Q_ENUM(Status)

    explicit DeclarativeCover(QQuickItem *parent = nullptr);

    QString status() const { return m_status; }
    bool allowResize() const { return m_allowResize; }
    void setAllowResize(bool allow);
    QSize size() const { return m_size; }
    bool transparent() const { return m_transparent; }
    void setTransparent(bool transparent);
    DeclarativeCoverActionArea* coverActionArea() const { return m_actionArea; }
    QQuickWindow* applicationWindow() const { return m_applicationWindow; }
    void setApplicationWindow(QQuickWindow *window);

    Q_INVOKABLE void tryResize(int width, int height, bool allowResize = false);

Q_SIGNALS:
    void statusChanged();
    void allowResizeChanged();
    void sizeChanged();
    void transparentChanged();
    void requestResize(int width, int height);
    void applicationWindowChanged();

private:
    void updateStatus();
    void updateSize();

    QString m_status;
    bool m_allowResize = false;
    QSize m_size;
    bool m_transparent = false;
    DeclarativeCoverActionArea *m_actionArea = nullptr;
    QQuickWindow *m_applicationWindow = nullptr;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECOVER_H
