/*
  ==============================================================================

    Knob.h
    Created: 23 Jan 2024 9:42:00pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

class CustomKnob : public juce::LookAndFeel_V4
{
public:
    CustomKnob();
    
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPosition, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &slider) override;
    
private:
    //== Images ====================================================================
    juce::Image knobImage;
};
