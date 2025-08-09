// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DRAGDETECTORITEM_H
#define SAILFISH_SILICA_PLUGIN_DRAGDETECTORITEM_H

#include <QQuickItem>
#include <QPointF>

class DragDetectorItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* flickable READ flickable WRITE setFlickable NOTIFY flickableChanged)
    Q_PROPERTY(bool horizontalDragUnused READ horizontalDragUnused NOTIFY horizontalDragUnusedChanged)
    Q_PROPERTY(bool verticalDragUnused READ verticalDragUnused NOTIFY verticalDragUnusedChanged)

public:
    explicit DragDetectorItem(QQuickItem *parent = nullptr);

    QQuickItem* flickable() const { return m_flickable; }
    void setFlickable(QQuickItem *flickable);
    bool horizontalDragUnused() const { return m_horizontalDragUnused; }
    bool verticalDragUnused() const { return m_verticalDragUnused; }

    Q_INVOKABLE void reset();

Q_SIGNALS:
    void flickableChanged();
    void horizontalDragUnusedChanged();
    void verticalDragUnusedChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void touchEvent(QTouchEvent *event) override;

private:
    void handlePress(const QPointF &pos);
    void handleMove(const QPointF &pos);
    void handleRelease();
    void updateDragUnused();

    QQuickItem *m_flickable = nullptr;
    QPointF m_pressPos;
    QPointF m_lastPos;
    bool m_dragging = false;
    bool m_horizontalDragUnused = false;
    bool m_verticalDragUnused = false;
    static constexpr qreal DRAG_THRESHOLD = 10.0;
};

#endif // SAILFISH_SILICA_PLUGIN_DRAGDETECTORITEM_H
