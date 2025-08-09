// SPDX-License-Identifier: LGPL-2.1-only

#include "notices.h"
#include <QCoreApplication>

Notices *Notices::s_instance = nullptr;

Notices::Notices(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &Notices::onTimerTimeout);
    s_instance = this;
}

Notices *Notices::instance()
{
    if (!s_instance) {
        s_instance = new Notices(QCoreApplication::instance());
    }
    return s_instance;
}

void Notices::show(const QString &text, int duration, Notice::Anchor anchor, 
                   qreal hOffset, qreal vOffset)
{
    NoticeData notice;
    notice.setText(text);
    notice.setDuration(duration);
    notice.setAnchor(anchor);
    notice.setHorizontalOffset(hOffset);
    notice.setVerticalOffset(vOffset);
    notice.setEmpty(false);

    scheduleNotice(notice);
}

void Notices::_dismissCurrent()
{
    if (!m_currentNotice.empty()) {
        m_timer.stop();
        m_currentNotice.setEmpty(true);
        emit currentNoticeChanged();
    }
}

void Notices::onTimerTimeout()
{
    _dismissCurrent();
}

void Notices::scheduleNotice(const NoticeData &notice)
{
    // Stop any existing timer
    m_timer.stop();

    // Set the new notice
    m_currentNotice = notice;
    emit currentNoticeChanged();

    // Start timer for auto-dismiss
    if (notice.duration() > 0) {
        m_timer.start(notice.duration());
    }
}
