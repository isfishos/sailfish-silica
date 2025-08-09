// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEDIALOGSTATUS_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEDIALOGSTATUS_H

#include <QObject>

class DeclarativeDialogStatus : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeDialogStatus(QObject *parent = nullptr);

    enum Status { Closed, Opening, Opened, Closing };
    Q_ENUM(Status)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEDIALOGSTATUS_H
