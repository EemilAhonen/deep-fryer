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
    // TODO: Make these initialise on the constructor
    juce::Colour _backgroundColor = juce::Colour(77,77,77);
    juce::Colour _fillColor = juce::Colour(195,52,63);
    juce::Colour _thumbColor = juce::Colour(251,185,15);
    
    float _rectVerticalScale = 0.2f;
    float _sliderThumbScale = 0.4f;
};
