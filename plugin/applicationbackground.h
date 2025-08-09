// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_APPLICATIONBACKGROUND_H
#define SAILFISH_SILICA_PLUGIN_APPLICATIONBACKGROUND_H

#include <QObject>
#include <QJSValue>
#include <QUrl>
#include <QColor>
#include <QPointer>

class QQmlComponent;
class QQuickWindow;

class ApplicationBackground : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJSValue wallpaper READ wallpaper WRITE setWallpaper NOTIFY patternChanged)
    Q_PROPERTY(QUrl image READ image WRITE setImage NOTIFY patternChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY patternChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY patternChanged)
    Q_PROPERTY(QString material READ material WRITE setMaterial NOTIFY patternChanged)
    Q_PROPERTY(QQmlComponent* _windowComponent READ windowComponent WRITE setWindowComponent NOTIFY windowComponentChanged)
public:
    explicit ApplicationBackground(QObject *parent = nullptr);

    QJSValue wallpaper() const { return m_wallpaper; }
    void setWallpaper(const QJSValue &v) { m_wallpaper=v; emit patternChanged(); updateWallpaperWindow(); }
    QUrl image() const { return m_image; }
    void setImage(const QUrl &u) { m_image=u; emit patternChanged(); updateWallpaperWindow(); }
    QString filter() const { return m_filter; }
    void setFilter(const QString &f) { m_filter=f; m_explicitFilter=true; emit patternChanged(); updateWallpaperWindow(); }
    QColor color() const { return m_color; }
    void setColor(const QColor &c) { m_color=c; emit patternChanged(); }
    QString material() const { return m_material; }
    void setMaterial(const QString &m) { m_material=m; emit patternChanged(); }
    QQmlComponent* windowComponent() const { return m_windowComponent; }
    void setWindowComponent(QQmlComponent *c) { m_windowComponent=c; emit windowComponentChanged(); }

    void setApplicationWindow(QQuickWindow *w);
    void complete();
    bool explicitFilter() const { return m_explicitFilter; }
    bool explicitColor() const { return m_explicitColor; }
    bool explicitMaterial() const { return m_explicitMaterial; }

Q_SIGNALS:
    void patternChanged();
    void windowComponentChanged();

private:
    void updateWallpaperWindow();
    void createWallpaperWindow();
    void destroyWallpaperWindow();

    QJSValue m_wallpaper;
    QUrl m_image;
    QString m_filter;
    QColor m_color;
    QString m_material;
    QQmlComponent *m_windowComponent = nullptr;
    QPointer<QQuickWindow> m_appWindow;
    QPointer<QQuickWindow> m_wallpaperWindow;
    bool m_explicitFilter = false;
    bool m_explicitColor = false;
    bool m_explicitMaterial = false;
    bool m_completed = false;
};

#endif // SAILFISH_SILICA_PLUGIN_APPLICATIONBACKGROUND_H
