// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_TIMEPICKERMODE_H
#define SAILFISH_SILICA_PLUGIN_TIMEPICKERMODE_H

#include <QObject>

class TimePickerMode : public QObject
{
    Q_OBJECT

public:
    enum Mode {
        Hours = 0,
        Minutes = 1
    };
    Q_ENUM(Mode)

    explicit TimePickerMode(QObject *parent = nullptr);
};

#endif // SAILFISH_SILICA_PLUGIN_TIMEPICKERMODE_H
