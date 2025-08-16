// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_VERTICALAUTOSCROLL_H
#define SAILFISH_SILICA_PLUGIN_VERTICALAUTOSCROLL_H

#include <qqml.h>
#include "autoscroll.h"

class VerticalAutoScroll : public AutoScroll
{
    Q_OBJECT
    Q_PROPERTY(qreal topMargin READ topMargin WRITE setTopMargin NOTIFY topMarginChanged)
    Q_PROPERTY(qreal bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged)
public:
    explicit VerticalAutoScroll(QObject *parent = nullptr);

    static VerticalAutoScroll *qmlAttachedProperties(QObject *object);

    qreal topMargin() const { return m_topMargin; }
    void setTopMargin(qreal v);

    qreal bottomMargin() const { return m_bottomMargin; }
    void setBottomMargin(qreal v);

Q_SIGNALS:
    void topMarginChanged();
    void bottomMarginChanged();

private:
    qreal m_topMargin = 0.0;
    qreal m_bottomMargin = 0.0;
};

QML_DECLARE_TYPEINFO(VerticalAutoScroll, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_VERTICALAUTOSCROLL_H
