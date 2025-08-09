// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEQUICKSCROLLBUTTONBASE_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEQUICKSCROLLBUTTONBASE_H

#include <QQuickItem>
#include <QPointF>

class DeclarativeQuickScrollButtonBase : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool pressed READ pressed NOTIFY pressedChanged)
    Q_PROPERTY(QQuickItem* flickable READ flickable WRITE setFlickable NOTIFY flickableChanged)

public:
    explicit DeclarativeQuickScrollButtonBase(QQuickItem *parent = nullptr);

    bool pressed() const { return m_pressed; }
    QQuickItem* flickable() const { return m_flickable; }
    void setFlickable(QQuickItem *flickable);

Q_SIGNALS:
    void pressedChanged();
    void flickableChanged();
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void touchEvent(QTouchEvent *event) override;

private:
    void handlePress(const QPointF &pos);
    void handleMove(const QPointF &pos);
    void handleRelease();
    bool isWithinTolerance(const QPointF &pos);

    bool m_pressed = false;
    QQuickItem *m_flickable = nullptr;
    QPointF m_pressPos;
    QPointF m_lastPos;
    bool m_tracking = false;
    static constexpr qreal CLICK_TOLERANCE = 10.0;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEQUICKSCROLLBUTTONBASE_H
