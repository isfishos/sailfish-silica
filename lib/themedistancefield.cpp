// SPDX-License-Identifier: LGPL-2.1-only

#include "themedistancefield.h"

namespace Silica {

// Heuristic SDF parameters tuned per pixel ratio; can be refined later
float ThemeDistanceField::thresholdFunction(float pixelRatio)
{
    // Base threshold around 0.5 with slight adjustment by pixel ratio
    // to keep glyph weight stable across densities
    if (pixelRatio <= 0.0f) return 0.5f;
    const float k = 0.06f; // tweak factor
    return 0.5f + k * (pixelRatio - 1.0f);
}

float ThemeDistanceField::antialiasingSpreadFunction(float pixelRatio)
{
    // Spread inversely proportional to pixel ratio to keep edge softness similar
    if (pixelRatio <= 0.0f) return 0.125f;
    const float base = 0.125f; // for ratio 1.0
    return base / pixelRatio;
}

}


