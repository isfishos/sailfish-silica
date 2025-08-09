// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_MINVERSEMOUSEAREA_H
#define SAILFISH_SILICA_PLUGIN_MINVERSEMOUSEAREA_H

#include <QQuickItem>
#include <QPointF>

class MInverseMouseArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool stealPress READ stealPress WRITE setStealPress NOTIFY stealPressChanged)

public:
    explicit MInverseMouseArea(QQuickItem *parent = nullptr);

    bool enabled() const { return m_enabled; }
    void setEnabled(bool enabled);
    bool stealPress() const { return m_stealPress; }
    void setStealPress(bool steal);

    Q_INVOKABLE void cancelTouch();

Q_SIGNALS:
    void pressedOutside(qreal x, qreal y);
    void clickedOutside(qreal x, qreal y);
    void enabledChanged();
    void stealPressChanged();

protected:
    void itemChange(ItemChange change, const ItemChangeData &value) override;

private:
    void installEventFilter();
    void removeEventFilter();
    bool eventFilter(QObject *obj, QEvent *event);
    bool isPointOutside(const QPointF &pos) const;

    bool m_enabled = true;
    bool m_stealPress = false;
    QQuickWindow *m_window = nullptr;
    QPointF m_pressPos;
    bool m_pressed = false;
    bool m_blockRecursion = false;
};

#endif // SAILFISH_SILICA_PLUGIN_MINVERSEMOUSEAREA_H
