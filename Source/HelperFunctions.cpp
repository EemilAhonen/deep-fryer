//
//  HelperFunctions.h
//  DeepFryer
//
//  Created by Eemil Ahonen on 23.1.2024.
//  Copyright © 2024 Keisari. All rights reserved.
//

#include "HelperFunctions.h"

//==============================================================================

const float clamp(const float value, const float minValue, const float maxValue)
{
    return (value < minValue) ? minValue : ((value > maxValue) ? maxValue : value);
}

const float max(const float a, const float b)
{
    return (a < b) ? b : a;
}
