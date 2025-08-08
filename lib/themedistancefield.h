// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SILICA_THEMEDISTANCEFIELD_H
#define SILICA_THEMEDISTANCEFIELD_H

#include <silicaglobal.h>

namespace Silica {

class SAILFISH_SILICA_EXPORT ThemeDistanceField {
public:
    static float thresholdFunction(float pixelRatio);
    static float antialiasingSpreadFunction(float pixelRatio);
};

}

#endif // SILICA_THEMEDISTANCEFIELD_H
