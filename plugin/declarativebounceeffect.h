// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEBOUNCEEFFECT_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEBOUNCEEFFECT_H

#include <QObject>
#include <QPointF>
#include <qqml.h>

QT_BEGIN_NAMESPACE
class QQuickFlickable;
QT_END_NAMESPACE

class DeclarativeBounceEffect : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)
    Q_PROPERTY(qreal difference READ difference NOTIFY differenceChanged)
    Q_PROPERTY(QQuickFlickable *flickable READ flickable WRITE setFlickable NOTIFY flickableChanged)
    Q_PROPERTY(QQmlListProperty<QObject> data READ data CONSTANT)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    explicit DeclarativeBounceEffect(QObject *parent = nullptr);

    bool active() const;
    qreal difference() const;
    QQuickFlickable *flickable() const;
    void setFlickable(QQuickFlickable *f);
    QQmlListProperty<QObject> data();

    Q_INVOKABLE void handlePress(const QPointF &pos);
    Q_INVOKABLE void handleMove(const QPointF &pos);
    Q_INVOKABLE void handleRelease();

Q_SIGNALS:
    void activeChanged();
    void differenceChanged();
    void flickableChanged();

private:
    QList<QObject*> m_data;
    bool m_active = false;
    qreal m_difference = 0.0;
    QQuickFlickable *m_flickable = nullptr;
    QPointF m_startPos;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEBOUNCEEFFECT_H
