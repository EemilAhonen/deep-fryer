//
//  AudioHelpers.cpp
//  DeepFryer
//
//  Created by Eemil Ahonen on 23.1.2024.
//  Copyright © 2024 Keisari. All rights reserved.
//

#include "AudioHelpers.h"

const float clamp(const float value, const float minValue, const float maxValue)
{
    // JUCE doesn't support std::clamp, hence this monstrosity
    return (value < minValue) ? minValue : ((value > maxValue) ? maxValue : value);
}

const float max(const float a, const float b)
{
    return (a < b) ? b : a;
}
