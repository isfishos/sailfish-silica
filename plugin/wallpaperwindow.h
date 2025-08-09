// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_WALLPAPERWINDOW_H
#define SAILFISH_SILICA_PLUGIN_WALLPAPERWINDOW_H

#include <QQuickWindow>
#include <QEvent>

class WallpaperWindow : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(bool windowVisible READ windowVisible WRITE setWindowVisible NOTIFY windowVisibleChanged)
    Q_PROPERTY(QQuickWindow *parentWindow READ parentWindow WRITE setParentWindow NOTIFY parentWindowChanged)
    Q_PROPERTY(qreal windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged)
    Q_PROPERTY(qreal windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged)

public:
    explicit WallpaperWindow(QQuickWindow *parent = nullptr);

    bool windowVisible() const { return m_windowVisible; }
    void setWindowVisible(bool visible);
    QQuickWindow *parentWindow() const { return m_parentWindow; }
    void setParentWindow(QQuickWindow *window);
    qreal windowWidth() const { return width(); }
    void setWindowWidth(qreal width);
    qreal windowHeight() const { return height(); }
    void setWindowHeight(qreal height);

Q_SIGNALS:
    void windowVisibleChanged();
    void parentWindowChanged();
    void windowWidthChanged();
    void windowHeightChanged();

protected:
    bool event(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void updateGeometry();

private:
    void syncWithParent();

    bool m_windowVisible = true;
    QQuickWindow *m_parentWindow = nullptr;
};

#endif // SAILFISH_SILICA_PLUGIN_WALLPAPERWINDOW_H
