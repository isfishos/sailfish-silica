// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEFOCUSBEHAVIOR_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEFOCUSBEHAVIOR_H

#include <QObject>

class DeclarativeFocusBehavior : public QObject
{
    Q_OBJECT

public:
    enum FocusBehavior {
        NoFocus = 0,
        FocusOnTap = 1,
        FocusOnPress = 2
    };
    Q_ENUM(FocusBehavior)

    explicit DeclarativeFocusBehavior(QObject *parent = nullptr);
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEFOCUSBEHAVIOR_H
