/*
 * Copyright (c) 2020 Open Mobile Platform LLC.
 *
 * License: Proprietary
*/

#ifndef SAILFISH_SILICA_BACKGROUND_FILL_P_H
#define SAILFISH_SILICA_BACKGROUND_FILL_P_H

#include <QObject>

namespace Sailfish { namespace Silica { namespace Background {

class Fill
{
    Q_GADGET
public:
    enum Mode {
        Stretch,
        PreserveAspectFit,
        PreserveAspectByExpanding,
    };
    Q_ENUM(Mode)
};

}}}

#endif
