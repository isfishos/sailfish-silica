// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_BACKGROUND_ABSTRACTFILTER_P_H
#define SAILFISH_SILICA_BACKGROUND_ABSTRACTFILTER_P_H

#include <QtGlobal>

namespace Sailfish { namespace Silica { namespace Background {

class AbstractFilter;

class AbstractFilterPrivate {
public:
    Q_DECLARE_PUBLIC(AbstractFilter)
    AbstractFilter *q_ptr = nullptr;
    bool enabled = true;
};

}}}

#endif // SAILFISH_SILICA_BACKGROUND_ABSTRACTFILTER_P_H
