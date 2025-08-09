// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEENTERKEY_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEENTERKEY_H

#include <QObject>
#include <QUrl>
#include <QString>

class QQuickItem;

class DeclarativeEnterKey : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl iconSource READ iconSource WRITE setIconSource NOTIFY iconSourceChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool highlighted READ highlighted WRITE setHighlighted NOTIFY highlightedChanged)

public:
    explicit DeclarativeEnterKey(QObject *parent = nullptr);

    QUrl iconSource() const { return m_iconSource; }
    void setIconSource(const QUrl &source);
    QString text() const { return m_text; }
    void setText(const QString &text);
    bool enabled() const { return m_enabled; }
    void setEnabled(bool enabled);
    bool highlighted() const { return m_highlighted; }
    void setHighlighted(bool highlighted);

    static DeclarativeEnterKey *qmlAttachedProperties(QObject *obj);

Q_SIGNALS:
    void clicked();
    void iconSourceChanged();
    void textChanged();
    void enabledChanged();
    void highlightedChanged();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void updateEditorExtension();
    QQuickItem* findEditorDelegate();

    QUrl m_iconSource;
    QString m_text;
    bool m_enabled = true;
    bool m_highlighted = false;
    QQuickItem *m_editor = nullptr;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEENTERKEY_H
