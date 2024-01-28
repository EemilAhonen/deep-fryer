/*
  ==============================================================================

    AdvancedKnob.h
    Created: 23 Jan 2024 9:42:00pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

class AdvancedKnob : public juce::LookAndFeel_V4
{
public:
    AdvancedKnob();
    
    void setParams(juce::Image knobImage);
    
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPosition, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &slider) override;
    
private:
    // TODO: Make these initialise on the constructor
    juce::Image _knobImage;
    juce::Colour _backgroundColor = juce::Colour(25, 25, 30);
    juce::Colour _outlineColor = juce::Colour(255, 255, 255);
    juce::Colour _fillColor = juce::Colour(195, 52, 63);
    juce::Colour _thumbColor = juce::Colour(251, 185, 15);
    
    float _innerScale = 0.84f;
    float _outlineWidth = 0.02f;
};
