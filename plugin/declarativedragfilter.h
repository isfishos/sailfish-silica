// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEDRAGFILTER_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEDRAGFILTER_H

#include <QQuickItem>
#include <QEvent>
#include <QPointF>

class DeclarativeDragFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool canceled READ canceled NOTIFY canceledChanged)
    Q_PROPERTY(qreal screenMargin READ screenMargin WRITE setScreenMargin NOTIFY screenMarginChanged)
    Q_PROPERTY(Qt::Orientations orientations READ orientations WRITE setOrientations NOTIFY orientationsChanged)

public:
    explicit DeclarativeDragFilter(QObject *parent = nullptr);

    bool canceled() const { return m_canceled; }
    qreal screenMargin() const { return m_screenMargin; }
    void setScreenMargin(qreal margin);
    Qt::Orientations orientations() const { return m_orientations; }
    void setOrientations(Qt::Orientations orientations);

    Q_INVOKABLE void begin(qreal x, qreal y);
    Q_INVOKABLE void end();
    Q_INVOKABLE void cancelClick();

    static DeclarativeDragFilter *qmlAttachedProperties(QObject *obj);

Q_SIGNALS:
    void canceledChanged();
    void screenMarginChanged();
    void orientationsChanged();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void resetState();
    bool isInScreenMargin(const QPointF &pos);
    bool exceedsDragThreshold(const QPointF &currentPos);

    bool m_canceled = false;
    qreal m_screenMargin = 0.0;
    Qt::Orientations m_orientations = Qt::Horizontal | Qt::Vertical;
    QPointF m_pressPos;
    QQuickItem *m_targetItem = nullptr;
    QQuickWindow *m_window = nullptr;
    bool m_tracking = false;
    static constexpr qreal DRAG_THRESHOLD = 10.0; // Platform drag threshold
};

QML_DECLARE_TYPEINFO(DeclarativeDragFilter, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEDRAGFILTER_H
