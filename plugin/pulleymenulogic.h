// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_PULLEYMENULOGIC_H
#define SAILFISH_SILICA_PLUGIN_PULLEYMENULOGIC_H

#include <QObject>
#include <QQmlParserStatus>
#include <QTimer>

class QQuickFlickable;

class PulleyMenuLogic : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool pullDownType READ pullDownType WRITE setPullDownType NOTIFY pullDownTypeChanged)
    Q_PROPERTY(QObject *flickable READ flickable WRITE setFlickable NOTIFY flickableChanged)
    Q_PROPERTY(qreal dragDistance READ dragDistance NOTIFY dragDistanceChanged)

public:
    explicit PulleyMenuLogic(QObject *parent = nullptr);

    bool pullDownType() const { return m_pullDownType; }
    void setPullDownType(bool pullDown);
    QObject *flickable() const { return m_flickable; }
    void setFlickable(QObject *flickable);
    qreal dragDistance() const { return m_dragDistance; }

    Q_INVOKABLE bool outOfBounds();
    Q_INVOKABLE void monitorFlick();

    // QQmlParserStatus implementation
    void classBegin() override {}
    void componentComplete() override;

Q_SIGNALS:
    void finalPositionReached();
    void flickableChanged();
    void pullDownTypeChanged();
    void animateFlick(int duration, qreal position);
    void dragDistanceChanged();

private slots:
    void onFlickablePropertyChanged();
    void onTimerTimeout();

private:
    void updateDragDistance();
    void connectToFlickable();

    bool m_pullDownType = true;
    QObject *m_flickable = nullptr;
    qreal m_dragDistance = 0.0;
    QTimer m_timer;
    bool m_monitoring = false;
};

#endif // SAILFISH_SILICA_PLUGIN_PULLEYMENULOGIC_H
