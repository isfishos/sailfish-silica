// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERACTION_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERACTION_H

#include <QObject>
#include <QUrl>

class DeclarativeCoverAction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl iconSource READ iconSource WRITE setIconSource NOTIFY iconSourceChanged)

public:
    explicit DeclarativeCoverAction(QObject *parent = nullptr);

    QUrl iconSource() const { return m_iconSource; }
    void setIconSource(const QUrl &source);

    Q_INVOKABLE void trigger();

Q_SIGNALS:
    void triggered();
    void iconSourceChanged();

private:
    QUrl m_iconSource;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVECOVERACTION_H
