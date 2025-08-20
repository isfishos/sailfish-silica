// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_NOTICES_H
#define SAILFISH_SILICA_PLUGIN_NOTICES_H

#include <QObject>
#include <QTimer>
#include <QPointer>
#include "notice.h"

class NoticeData
{
    Q_GADGET
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(int duration READ duration WRITE setDuration)
    Q_PROPERTY(Notice::Anchor anchor READ anchor WRITE setAnchor)
    Q_PROPERTY(qreal horizontalOffset READ horizontalOffset WRITE setHorizontalOffset)
    Q_PROPERTY(qreal verticalOffset READ verticalOffset WRITE setVerticalOffset)
    Q_PROPERTY(bool empty READ empty WRITE setEmpty)

public:
    NoticeData() : m_duration(Notice::Short), m_anchor(Notice::Center), 
                   m_horizontalOffset(0.0), m_verticalOffset(0.0), m_empty(true) {}

    QString text() const { return m_text; }
    void setText(const QString &text) { m_text = text; }
    int duration() const { return m_duration; }
    void setDuration(int duration) { m_duration = duration; }
    Notice::Anchor anchor() const { return m_anchor; }
    void setAnchor(Notice::Anchor anchor) { m_anchor = anchor; }
    qreal horizontalOffset() const { return m_horizontalOffset; }
    void setHorizontalOffset(qreal offset) { m_horizontalOffset = offset; }
    qreal verticalOffset() const { return m_verticalOffset; }
    void setVerticalOffset(qreal offset) { m_verticalOffset = offset; }
    bool empty() const { return m_empty; }
    void setEmpty(bool empty) { m_empty = empty; }

private:
    QString m_text;
    int m_duration;
    Notice::Anchor m_anchor;
    qreal m_horizontalOffset;
    qreal m_verticalOffset;
    bool m_empty;
};

Q_DECLARE_METATYPE(NoticeData)

class Notices : public QObject
{
    Q_OBJECT
    Q_PROPERTY(NoticeData _currentNotice READ currentNotice NOTIFY currentNoticeChanged)

public:
    explicit Notices(QObject *parent = nullptr);
    static Notices *instance();

    NoticeData currentNotice() const { return m_currentNotice; }

    Q_INVOKABLE void show(const QString &text, int duration = Notice::Short, 
                         Notice::Anchor anchor = Notice::Center, 
                         qreal hOffset = 0.0, qreal vOffset = 0.0);
    Q_INVOKABLE void _dismissCurrent();

Q_SIGNALS:
    void currentNoticeChanged();

private slots:
    void onTimerTimeout();

private:
    void scheduleNotice(const NoticeData &notice);

    NoticeData m_currentNotice;
    QTimer m_timer;
    static Notices *s_instance;
};

#endif // SAILFISH_SILICA_PLUGIN_NOTICES_H
