/*
  ==============================================================================

    SimpleSlider.cpp
    Created: 24 Jan 2024 8:15:13pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "SimpleSlider.h"

//==============================================================================

SimpleSlider::SimpleSlider()
{
    
}

int SimpleSlider::getSliderThumbRadius(juce::Slider& slider)
{
    return (int)(slider.getHeight() * _sliderThumbScale);
}

void SimpleSlider::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                       float sliderPos, float minSliderPos, float maxSliderPos,
                                   juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // Calculate scaled dimensions and positions
    float cornerSize = height * _rectVerticalScale * 0.35f;
    float rectangleWidth = width;
    float rectangleHeight = height * _rectVerticalScale;
    float rectangleY = y + height / 2 * (1 - _rectVerticalScale);
    
    float thumbDiameter = height * _sliderThumbScale;
    float thumbY = y + height / 2 * (1 - _sliderThumbScale);
    
    float thumbSlider = sliderPos - thumbDiameter / 2.0f;
    float foreSlider = sliderPos - thumbDiameter;
    
    // Draw background
    g.setColour(_backgroundColor);
    g.fillRoundedRectangle(x, rectangleY, rectangleWidth, rectangleHeight, cornerSize);
    
    // Draw fill
    g.setColour(_fillColor);
    g.fillRoundedRectangle(x, rectangleY, foreSlider, rectangleHeight, cornerSize);
    
    // Draw slider thumb
    g.setColour(_thumbColor);
    g.fillEllipse(thumbSlider, thumbY, thumbDiameter, thumbDiameter);
}
