/*
  ==============================================================================

    Parameters.h
    Created: 27 Jan 2024 8:37:05pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Keisari-Modules/UI-Components/SliderComponent.h"

//==============================================================================

class Parameters
{
public:
    Parameters();
    std::vector<std::unique_ptr<SliderComponent>>& getSliderComponents() { return _sliderComponents; }
    std::vector<std::unique_ptr<juce::RangedAudioParameter>>& getAudioParameters() { return _audioParameters; }
    
private:
    void initParameters();
    void addSliderParameter(const juce::String id, const juce::String name, float minValue, float maxValue, float initValue, float interval, float x, float y, float width, float height, int lookAndFeelID);
    
    std::vector<std::unique_ptr<SliderComponent>> _sliderComponents;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> _audioParameters;
};
