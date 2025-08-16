// SPDX-License-Identifier: LGPL-2.1-only

#include "horizontalautoscroll.h"

HorizontalAutoScroll::HorizontalAutoScroll(QObject *parent)
    : AutoScroll(parent)
{
}

HorizontalAutoScroll *HorizontalAutoScroll::qmlAttachedProperties(QObject *object)
{
    return new HorizontalAutoScroll(object);
}

void HorizontalAutoScroll::setLeftMargin(qreal v)
{
    if (!qFuzzyCompare(m_leftMargin, v)) { m_leftMargin = v; emit leftMarginChanged(); }
}

void HorizontalAutoScroll::setRightMargin(qreal v)
{
    if (!qFuzzyCompare(m_rightMargin, v)) { m_rightMargin = v; emit rightMarginChanged(); }
}
