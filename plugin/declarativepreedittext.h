// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEPREEDITTEXT_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEPREEDITTEXT_H

#include <QObject>
#include <QQmlParserStatus>
#include <QPointer>

class QQuickItem;

class DeclarativePreeditText : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    explicit DeclarativePreeditText(QObject *parent = nullptr);

    QString text() const { return m_text; }
    void setText(const QString &text);

    void classBegin() override {}
    void componentComplete() override;

Q_SIGNALS:
    void textChanged();

private:
    void detectParentEditor();
    void handleFocusChanged();
    void updatePreeditText();

    QString m_text;
    QPointer<QQuickItem> m_parentEditor;
    bool m_recursionGuard = false;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEPREEDITTEXT_H
