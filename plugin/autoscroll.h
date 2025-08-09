// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_AUTOSCROLL_H
#define SAILFISH_SILICA_PLUGIN_AUTOSCROLL_H

#include <QObject>
#include <QRectF>

class AutoScroll : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool keepVisible READ keepVisible WRITE setKeepVisible NOTIFY keepVisibleChanged)
    Q_PROPERTY(bool restorePosition READ restorePosition WRITE setRestorePosition NOTIFY restorePositionChanged)
    Q_PROPERTY(bool modal READ modal WRITE setModal NOTIFY modalChanged)
    Q_PROPERTY(bool animated READ animated WRITE setAnimated NOTIFY animatedChanged)
    Q_PROPERTY(bool hasCursor READ hasCursor NOTIFY hasCursorChanged)
    Q_PROPERTY(QRectF cursorRectangle READ cursorRectangle WRITE setCursorRectangle NOTIFY cursorRectangleChanged)
public:
    explicit AutoScroll(QObject *parent = nullptr);

    bool keepVisible() const;
    void setKeepVisible(bool v);
    bool restorePosition() const;
    void setRestorePosition(bool v);
    bool modal() const;
    void setModal(bool v);
    bool animated() const;
    void setAnimated(bool v);
    bool hasCursor() const;
    QRectF cursorRectangle() const;
    void setCursorRectangle(const QRectF &r);

    Q_INVOKABLE void fixup();
    Q_INVOKABLE void scroll();

Q_SIGNALS:
    void keepVisibleChanged();
    void restorePositionChanged();
    void modalChanged();
    void animatedChanged();
    void hasCursorChanged();
    void cursorRectangleChanged();

private:
    bool m_keepVisible = false;
    bool m_restorePosition = false;
    bool m_modal = false;
    bool m_animated = false;
    bool m_hasCursor = false;
    QRectF m_cursorRect;
};

#endif // SAILFISH_SILICA_PLUGIN_AUTOSCROLL_H
