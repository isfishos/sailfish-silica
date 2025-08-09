// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERWINDOW_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERWINDOW_H

#include <QQuickWindow>
#include <QSize>

class DeclarativeCoverWindow : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(QSize coverSize READ coverSize WRITE setCoverSize NOTIFY coverSizeChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)

public:
    explicit DeclarativeCoverWindow(QWindow *parent = nullptr);

    QSize coverSize() const { return m_coverSize; }
    void setCoverSize(const QSize &size);

Q_SIGNALS:
    void coverSizeChanged();
    void visibleChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    void updateGeometry();
    void syncWithMainWindow();

    QSize m_coverSize;
    QQuickWindow *m_mainWindow = nullptr;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERWINDOW_H
