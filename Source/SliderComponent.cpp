/*
  ==============================================================================

    SliderComponent.cpp
    Created: 27 Jan 2024 3:27:03am
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "SliderComponent.h"

void SliderComponent::setBounds(float scaleFactor)
{
    _slider.setBounds(_x * scaleFactor, _y * scaleFactor, _width * scaleFactor, _height * scaleFactor);
}
