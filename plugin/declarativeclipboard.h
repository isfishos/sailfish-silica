// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECLIPBOARD_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECLIPBOARD_H

#include <QObject>
#include <QString>

class DeclarativeClipboard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool hasText READ hasText NOTIFY hasTextChanged)
public:
    explicit DeclarativeClipboard(QObject *parent = nullptr);
    QString text() const;
    void setText(const QString &t);
    bool hasText() const;

Q_SIGNALS:
    void textChanged();
    void hasTextChanged();

private:
    QString m_text;
    bool m_hasText = false;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECLIPBOARD_H
