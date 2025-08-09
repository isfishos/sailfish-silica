// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTATUS_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTATUS_H

#include <QObject>

class DeclarativePageStatus : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativePageStatus(QObject *parent = nullptr);

    enum Status {
        Inactive,
        Activating,
        Active,
        Deactivating
    };
    Q_ENUM(Status)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEPAGESTATUS_H
