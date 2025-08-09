// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEBUSYINDICATORSIZE_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEBUSYINDICATORSIZE_H

#include <QObject>

class DeclarativeBusyIndicatorSize : public QObject
{
    Q_OBJECT

public:
    enum Size {
        Small = 0,
        Medium = 1,
        Large = 2
    };
    Q_ENUM(Size)

    explicit DeclarativeBusyIndicatorSize(QObject *parent = nullptr);
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEBUSYINDICATORSIZE_H
