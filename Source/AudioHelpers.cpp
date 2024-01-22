//
//  AudioHelpers.cpp
//  DeepFryer
//
//  Created by Eemil Ahonen on 23.1.2024.
//  Copyright © 2024 Keisari. All rights reserved.
//

#include "AudioHelpers.h"

float clampValue(float value, float minValue, float maxValue)
{
    // JUCE doesn't support std::clamp, hence this monstrosity
    if (value < minValue)
    {
        return minValue;
    }
    else if (value > maxValue)
    {
        return maxValue;
    }
    return value;
}

float saturateSample(float sample, float driveValue, float ceilingValue)
{
    float saturatedSample = sample * driveValue;
    return clampValue(saturatedSample, -ceilingValue, ceilingValue);
}
