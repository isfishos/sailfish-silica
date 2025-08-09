// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_TIMEZONEUPDATER_H
#define SAILFISH_SILICA_PLUGIN_TIMEZONEUPDATER_H

#include <QObject>
#include <QString>

class QQmlEngine;

class TimezoneUpdater : public QObject
{
    Q_OBJECT

public:
    explicit TimezoneUpdater(QQmlEngine *engine, QObject *parent = nullptr);
    ~TimezoneUpdater();

private slots:
    void onTimezoneChanged();

private:
    QQmlEngine *m_engine;
    QString m_lastTimezone;
};

#endif // SAILFISH_SILICA_PLUGIN_TIMEZONEUPDATER_H
