// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEDOCK_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEDOCK_H

#include <QObject>

class DeclarativeDock : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeDock(QObject *parent = nullptr);

    enum Dock {
        Top = 1,
        Bottom = 2,
        Left = 4,
        Right = 8
    };
    Q_ENUM(Dock)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEDOCK_H
