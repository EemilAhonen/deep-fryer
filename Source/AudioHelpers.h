//
//  AudioHelpers.h
//  DeepFryer
//
//  Created by Eemil Ahonen on 23.1.2024.
//  Copyright © 2024 Keisari. All rights reserved.
//

#pragma once

#include <JuceHeader.h>

float clampValue(float value, float minValue, float maxValue);
float saturateSample(float sample, float driveValue, float ceilingValue);
