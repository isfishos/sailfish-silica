// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_HORIZONTALAUTOSCROLL_H
#define SAILFISH_SILICA_PLUGIN_HORIZONTALAUTOSCROLL_H

#include <qqml.h>
#include "autoscroll.h"

class HorizontalAutoScroll : public AutoScroll
{
    Q_OBJECT
    Q_PROPERTY(qreal leftMargin READ leftMargin WRITE setLeftMargin NOTIFY leftMarginChanged)
    Q_PROPERTY(qreal rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged)
public:
    explicit HorizontalAutoScroll(QObject *parent = nullptr);

    static HorizontalAutoScroll *qmlAttachedProperties(QObject *object);

    qreal leftMargin() const { return m_leftMargin; }
    void setLeftMargin(qreal v);

    qreal rightMargin() const { return m_rightMargin; }
    void setRightMargin(qreal v);

Q_SIGNALS:
    void leftMarginChanged();
    void rightMarginChanged();

private:
    qreal m_leftMargin = 0.0;
    qreal m_rightMargin = 0.0;
};

QML_DECLARE_TYPEINFO(HorizontalAutoScroll, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_HORIZONTALAUTOSCROLL_H
