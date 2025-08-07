// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_BACKGROUND_CONVOLUTIONFILTER_P_H
#define SAILFISH_SILICA_BACKGROUND_CONVOLUTIONFILTER_P_H

#include "abstractfilter_p.h"
#include "convolutionfilter.h"

namespace Sailfish { namespace Silica { namespace Background {

class ConvolutionFilterPrivate : public AbstractFilterPrivate {
public:
    Kernel kernel;
};

}}}

#endif // SAILFISH_SILICA_BACKGROUND_CONVOLUTIONFILTER_P_H


