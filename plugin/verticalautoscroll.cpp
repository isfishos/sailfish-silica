// SPDX-License-Identifier: LGPL-2.1-only

#include "verticalautoscroll.h"

VerticalAutoScroll::VerticalAutoScroll(QObject *parent)
    : AutoScroll(parent)
{
}

VerticalAutoScroll *VerticalAutoScroll::qmlAttachedProperties(QObject *object)
{
    return new VerticalAutoScroll(object);
}

void VerticalAutoScroll::setTopMargin(qreal v)
{
    if (!qFuzzyCompare(m_topMargin, v)) { m_topMargin = v; emit topMarginChanged(); }
}

void VerticalAutoScroll::setBottomMargin(qreal v)
{
    if (!qFuzzyCompare(m_bottomMargin, v)) { m_bottomMargin = v; emit bottomMarginChanged(); }
}
