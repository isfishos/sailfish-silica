// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEDIALOGRESULT_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEDIALOGRESULT_H

#include <QObject>

class DeclarativeDialogResult : public QObject
{
    Q_OBJECT
public:
    explicit DeclarativeDialogResult(QObject *parent = nullptr);

    enum Result { None, Accepted, Rejected };
    Q_ENUM(Result)
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEDIALOGRESULT_H
