// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEDATETIME_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEDATETIME_H

#include <QObject>

class DeclarativeDateTime : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeDateTime(QObject *parent = nullptr);

    enum HourMode { DefaultHours, TwentyFourHours, TwelveHours };
    Q_ENUM(HourMode)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEDATETIME_H
