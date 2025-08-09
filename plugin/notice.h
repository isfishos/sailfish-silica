// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_NOTICE_H
#define SAILFISH_SILICA_PLUGIN_NOTICE_H

#include <QObject>
#include <QPointF>

class Notice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Anchor anchor READ anchor WRITE setAnchor NOTIFY anchorChanged)
    Q_PROPERTY(qreal horizontalOffset READ horizontalOffset WRITE setHorizontalOffset NOTIFY horizontalOffsetChanged)
    Q_PROPERTY(qreal verticalOffset READ verticalOffset WRITE setVerticalOffset NOTIFY verticalOffsetChanged)
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    enum Anchor {
        Center = 0x00,
        Left = 0x01,
        Right = 0x02,
        Top = 0x04,
        Bottom = 0x08
    };
    Q_ENUM(Anchor)
    Q_DECLARE_FLAGS(Anchors, Anchor)

    enum Duration {
        Short = 3000,
        Long = 5000
    };
    Q_ENUM(Duration)

    explicit Notice(QObject *parent = nullptr);

    Anchor anchor() const { return m_anchor; }
    void setAnchor(Anchor anchor);
    qreal horizontalOffset() const { return m_horizontalOffset; }
    void setHorizontalOffset(qreal offset);
    qreal verticalOffset() const { return m_verticalOffset; }
    void setVerticalOffset(qreal offset);
    int duration() const { return m_duration; }
    void setDuration(int duration);
    QString text() const { return m_text; }
    void setText(const QString &text);

    Q_INVOKABLE void show();

Q_SIGNALS:
    void anchorChanged();
    void horizontalOffsetChanged();
    void verticalOffsetChanged();
    void durationChanged();
    void textChanged();

private:
    Anchor m_anchor = Center;
    qreal m_horizontalOffset = 0.0;
    qreal m_verticalOffset = 0.0;
    int m_duration = Short;
    QString m_text;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Notice::Anchors)

#endif // SAILFISH_SILICA_PLUGIN_NOTICE_H
