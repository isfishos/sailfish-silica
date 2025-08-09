// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_AUTOSCROLLCONTROLLER_H
#define SAILFISH_SILICA_PLUGIN_AUTOSCROLLCONTROLLER_H

#include <QObject>
#include <QQmlListProperty>
#include <qqml.h>
#include <QList>

class QQuickItem;
class QQuickFlickable;

class AutoScrollController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickFlickable* flickable READ flickable CONSTANT)
    Q_PROPERTY(QQuickItem* horizontalScrollItem READ horizontalScrollItem CONSTANT)
    Q_PROPERTY(QQuickItem* verticalScrollItem READ verticalScrollItem CONSTANT)
    Q_PROPERTY(bool active READ active CONSTANT)
    Q_PROPERTY(bool modal READ modal CONSTANT)
    Q_PROPERTY(QQmlListProperty<QObject> data READ data CONSTANT)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    explicit AutoScrollController(QObject *parent = nullptr);

    static AutoScrollController *qmlAttachedProperties(QObject *target);

    QQuickFlickable* flickable() const { return nullptr; }
    QQuickItem* horizontalScrollItem() const { return nullptr; }
    QQuickItem* verticalScrollItem() const { return nullptr; }
    bool active() const { return false; }
    bool modal() const { return false; }
    QQmlListProperty<QObject> data();

    Q_INVOKABLE void scheduleHorizontalScroll() {}
    Q_INVOKABLE void scheduleVerticalScroll() {}

Q_SIGNALS:
    void scrollHorizontally(qreal x, bool animated);
    void scrollVertically(qreal y, bool animated);

private:
    void appendChild(QObject *obj) { m_children.append(obj); }
    int childCount() const { return m_children.size(); }
    QObject *childAt(int idx) const { return m_children.value(idx); }
    void clearChildren() { m_children.clear(); }
    QList<QObject*> m_children;
};

QML_DECLARE_TYPEINFO(AutoScrollController, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_AUTOSCROLLCONTROLLER_H
