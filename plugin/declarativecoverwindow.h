// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERWINDOW_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERWINDOW_H

#include <QQuickWindow>
#include <QSize>

class DeclarativeCover;

class DeclarativeCoverWindow : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(QSize coverSize READ coverSize WRITE setCoverSize NOTIFY coverSizeChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QObject* mainWindow READ mainWindow WRITE setMainWindow NOTIFY mainWindowChanged)
    Q_PROPERTY(DeclarativeCover* cover READ cover WRITE setCover NOTIFY coverChanged)

public:
    explicit DeclarativeCoverWindow(QWindow *parent = nullptr);

    QSize coverSize() const { return m_coverSize; }
    void setCoverSize(const QSize &size);

    QObject* mainWindow() const { return m_mainWindow; }
    void setMainWindow(QObject *w);

    DeclarativeCover* cover() const { return m_cover; }
    void setCover(DeclarativeCover *c) { if (m_cover != c) { m_cover = c; emit coverChanged(); } }

    Q_INVOKABLE void setContentSize(qreal w, qreal h);

Q_SIGNALS:
    void coverSizeChanged();
    void visibleChanged();
    void mainWindowChanged();
    void coverChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    void updateGeometry();
    void syncWithMainWindow();

    QSize m_coverSize;
    QWindow *m_mainWindow = nullptr;
    DeclarativeCover *m_cover = nullptr;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERWINDOW_H
