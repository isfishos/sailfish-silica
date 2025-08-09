// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_SWIPEGESTUREAREA_H
#define SAILFISH_SILICA_PLUGIN_SWIPEGESTUREAREA_H

#include <QQuickItem>
#include <QPointF>

class SwipeGestureArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool swipeEnabled READ swipeEnabled WRITE setSwipeEnabled NOTIFY swipeEnabledChanged)
    Q_PROPERTY(int thresholdX READ thresholdX WRITE setThresholdX NOTIFY thresholdXChanged)
    Q_PROPERTY(int thresholdY READ thresholdY WRITE setThresholdY NOTIFY thresholdYChanged)
    Q_PROPERTY(int swipeAmount READ swipeAmount NOTIFY swipeAmountChanged)
    Q_PROPERTY(bool gestureInProgress READ gestureInProgress NOTIFY gestureInProgressChanged)
    Q_PROPERTY(Direction direction READ direction NOTIFY directionChanged)
    Q_PROPERTY(Directions allowedDirections READ allowedDirections WRITE setAllowedDirections NOTIFY allowedDirectionsChanged)
    Q_PROPERTY(Directions forceDirections READ forceDirections WRITE setForceDirections NOTIFY forceDirectionsChanged)

public:
    enum Direction {
        None = 0x00,
        Left = 0x01,
        Right = 0x02,
        Up = 0x04,
        Down = 0x08,
        Horizontal = Left | Right,
        Vertical = Up | Down,
        All = Horizontal | Vertical
    };
    Q_ENUM(Direction)
    Q_DECLARE_FLAGS(Directions, Direction)

    explicit SwipeGestureArea(QQuickItem *parent = nullptr);

    bool swipeEnabled() const { return m_swipeEnabled; }
    void setSwipeEnabled(bool enabled);
    int thresholdX() const { return m_thresholdX; }
    void setThresholdX(int threshold);
    int thresholdY() const { return m_thresholdY; }
    void setThresholdY(int threshold);
    int swipeAmount() const { return m_swipeAmount; }
    bool gestureInProgress() const { return m_gestureInProgress; }
    Direction direction() const { return m_direction; }
    Directions allowedDirections() const { return m_allowedDirections; }
    void setAllowedDirections(Directions directions);
    Directions forceDirections() const { return m_forceDirections; }
    void setForceDirections(Directions directions);

    Q_INVOKABLE void endGesture();

Q_SIGNALS:
    void swipeEnabledChanged();
    void thresholdXChanged();
    void thresholdYChanged();
    void swipeAmountChanged();
    void gestureInProgressChanged();
    void directionChanged();
    void allowedDirectionsChanged();
    void forceDirectionsChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool childMouseEventFilter(QQuickItem *item, QEvent *event) override;

private:
    void resetGesture();
    Direction determineDirection(qreal dx, qreal dy);
    bool isDirectionAllowed(Direction direction) const;

    bool m_swipeEnabled = true;
    int m_thresholdX = 50;
    int m_thresholdY = 50;
    int m_swipeAmount = 0;
    bool m_gestureInProgress = false;
    Direction m_direction = None;
    Directions m_allowedDirections = All;
    Directions m_forceDirections = None;

    QPointF m_startPos;
    QPointF m_lastPos;
    bool m_pressed = false;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SwipeGestureArea::Directions)

#endif // SAILFISH_SILICA_PLUGIN_SWIPEGESTUREAREA_H
