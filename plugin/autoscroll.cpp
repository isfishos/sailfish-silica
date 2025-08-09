// SPDX-License-Identifier: LGPL-2.1-only

#include "autoscroll.h"

AutoScroll::AutoScroll(QObject *parent)
    : QObject(parent)
{
}

bool AutoScroll::keepVisible() const
{
    return m_keepVisible;
}

void AutoScroll::setKeepVisible(bool v)
{
    if (m_keepVisible == v)
        return;
    m_keepVisible = v;
    Q_EMIT keepVisibleChanged();
}

bool AutoScroll::restorePosition() const
{
    return m_restorePosition;
}

void AutoScroll::setRestorePosition(bool v)
{
    if (m_restorePosition == v)
        return;
    m_restorePosition = v;
    Q_EMIT restorePositionChanged();
}

bool AutoScroll::modal() const
{
    return m_modal;
}

void AutoScroll::setModal(bool v)
{
    if (m_modal == v)
        return;
    m_modal = v;
    Q_EMIT modalChanged();
}

bool AutoScroll::animated() const
{
    return m_animated;
}

void AutoScroll::setAnimated(bool v)
{
    if (m_animated == v)
        return;
    m_animated = v;
    Q_EMIT animatedChanged();
}

bool AutoScroll::hasCursor() const
{
    return m_hasCursor;
}

QRectF AutoScroll::cursorRectangle() const
{
    return m_cursorRect;
}

void AutoScroll::setCursorRectangle(const QRectF &r)
{
    if (m_cursorRect == r)
        return;
    m_cursorRect = r;
    Q_EMIT cursorRectangleChanged();
}

void AutoScroll::fixup() {}
void AutoScroll::scroll() {}
