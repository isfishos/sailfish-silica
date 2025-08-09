// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVETOUCHBLOCKER_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVETOUCHBLOCKER_H

#include <QQuickItem>

class DeclarativeTouchBlocker : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* target READ target WRITE setTarget NOTIFY targetChanged)

public:
    explicit DeclarativeTouchBlocker(QQuickItem *parent = nullptr);

    QQuickItem* target() const { return m_target; }
    void setTarget(QQuickItem *target);

Q_SIGNALS:
    void targetChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void touchEvent(QTouchEvent *event) override;

private:
    void installEventFilter();
    void removeEventFilter();

    QQuickItem *m_target = nullptr;
    bool m_eventFilterInstalled = false;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVETOUCHBLOCKER_H
