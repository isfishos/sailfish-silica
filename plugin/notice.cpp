// SPDX-License-Identifier: LGPL-2.1-only

#include "notice.h"
#include "notices.h"

Notice::Notice(QObject *parent)
    : QObject(parent)
{
}

void Notice::setAnchor(Anchor anchor)
{
    if (m_anchor != anchor) {
        m_anchor = anchor;
        emit anchorChanged();
    }
}

void Notice::setHorizontalOffset(qreal offset)
{
    if (!qFuzzyCompare(m_horizontalOffset, offset)) {
        m_horizontalOffset = offset;
        emit horizontalOffsetChanged();
    }
}

void Notice::setVerticalOffset(qreal offset)
{
    if (!qFuzzyCompare(m_verticalOffset, offset)) {
        m_verticalOffset = offset;
        emit verticalOffsetChanged();
    }
}

void Notice::setDuration(int duration)
{
    if (m_duration != duration) {
        m_duration = duration;
        emit durationChanged();
    }
}

void Notice::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        emit textChanged();
    }
}

void Notice::show()
{
    if (Notices *notices = Notices::instance()) {
        notices->show(m_text, m_duration, m_anchor, m_horizontalOffset, m_verticalOffset);
    }
}
