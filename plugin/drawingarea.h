// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DRAWINGAREA_H
#define SAILFISH_SILICA_PLUGIN_DRAWINGAREA_H

#include "lineitem.h"
#include <QVector>
#include <QPointF>

class DrawingArea : public LineItem
{
    Q_OBJECT
    Q_PROPERTY(qreal threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(QQuickItem* mask READ mask WRITE setMask NOTIFY maskChanged)
    Q_PROPERTY(int maximumStrokeCount READ maximumStrokeCount WRITE setMaximumStrokeCount NOTIFY maximumStrokeCountChanged)

public:
    explicit DrawingArea(QQuickItem *parent = nullptr);

    qreal threshold() const { return m_threshold; }
    void setThreshold(qreal threshold);
    QQuickItem* mask() const { return m_mask; }
    void setMask(QQuickItem *mask);
    int maximumStrokeCount() const { return m_maximumStrokeCount; }
    void setMaximumStrokeCount(int count);

    Q_INVOKABLE void clear();

Q_SIGNALS:
    void thresholdChanged();
    void maskChanged();
    void maximumStrokeCountChanged();
    void arcStarted(qreal x, qreal y);
    void arcPointAdded(qreal x, qreal y);
    void arcFinished();
    void arcCanceled();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void touchEvent(QTouchEvent *event) override;

private:
    void handlePress(const QPointF &pos);
    void handleMove(const QPointF &pos);
    void handleRelease();
    bool isPointInMask(const QPointF &pos) const;
    void addStroke(const QVector<QPointF> &points);

    qreal m_threshold = 5.0;
    QQuickItem *m_mask = nullptr;
    int m_maximumStrokeCount = 10;
    QVector<QVector<QPointF>> m_strokes;
    QVector<QPointF> m_currentStroke;
    QPointF m_lastPoint;
    bool m_drawing = false;
    int m_touchId = -1;
};

#endif // SAILFISH_SILICA_PLUGIN_DRAWINGAREA_H
