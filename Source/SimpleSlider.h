/*
  ==============================================================================

    SimpleSlider.h
    Created: 24 Jan 2024 8:15:13pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

class SimpleSlider : public juce::LookAndFeel_V4
{
public:
    SimpleSlider();
    
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                               float sliderPos, float minSliderPos, float maxSliderPos,
                           juce::Slider::SliderStyle style, juce::Slider& slider) override;
    
    int getSliderThumbRadius(juce::Slider& slider) override;

private:
    juce::Colour backgroundColor;
    juce::Colour foregroundColor;
    juce::Colour thumbColor;
    
    float rectVerticalScale = 0.2f;
    float sliderThumbScale = 0.4f;
};
