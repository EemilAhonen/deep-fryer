/*
  ==============================================================================

    CustomSlider.cpp
    Created: 24 Jan 2024 8:15:13pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "SimpleSlider.h"

//==============================================================================

SimpleSlider::SimpleSlider()
{
    // TODO: Init these on call
    backgroundColor = juce::Colour(77,77,77);
    foregroundColor = juce::Colour(195,52,63);
    thumbColor = juce::Colour(251,185,15);
    
    rectVerticalScale = 0.2f;
    sliderThumbScale = 0.4f;
}

int SimpleSlider::getSliderThumbRadius(juce::Slider& slider)
{
    return (int)(slider.getHeight() * sliderThumbScale);
}

void SimpleSlider::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                       float sliderPos, float minSliderPos, float maxSliderPos,
                                   juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // Scale the slider according to the sliderThumb and rectVertical scale
    float cornerSize = height * rectVerticalScale * 0.35f;
    float rectangleWidth = width;
    float rectangleHeight = height * rectVerticalScale;
    float rectangleY = y + height / 2 * (1 - rectVerticalScale);
    
    float thumbDiameter = height * sliderThumbScale;
    float thumbY = y + height / 2 * (1 - sliderThumbScale);
    
    float thumbSlider = sliderPos - thumbDiameter / 2.0f;
    float foreSlider = sliderPos - thumbDiameter;
    
    // Draw background
    g.setColour(backgroundColor);
    g.fillRoundedRectangle(x, rectangleY, rectangleWidth, rectangleHeight, cornerSize);
    
    // Draw foreground
    g.setColour(foregroundColor);
    g.fillRoundedRectangle(x, rectangleY, foreSlider, rectangleHeight, cornerSize);
    
    // Draw slider thumb
    g.setColour(thumbColor);
    g.fillEllipse(thumbSlider, thumbY, thumbDiameter, thumbDiameter);
}
