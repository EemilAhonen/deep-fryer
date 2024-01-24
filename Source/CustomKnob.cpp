/*
  ==============================================================================

    Knob.cpp
    Created: 23 Jan 2024 9:42:00pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "CustomKnob.h"

//==============================================================================

CustomKnob::CustomKnob()
{
    // TODO: Init these on call
    knobImage = juce::ImageCache::getFromMemory(BinaryData::Knob_Front_png, BinaryData::Background_pngSize);
}

void CustomKnob::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPosition, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &slider)
{
    // TODO: FINISH THIS
    
    float diameter = fmin(width, height) * 0.84f;
    float radius = diameter * 0.5;
    float centerX = x + width * 0.5;
    float centerY = y + height * 0.5;
    float rx = centerX - radius;
    float ry = centerY - radius;
    float angle = rotaryStartAngle + (sliderPosition * (rotaryEndAngle - rotaryStartAngle));
    float imageSize = knobImage.getHeight();
    float scaleFactor = diameter / (imageSize);
    
    float backgroundScale = 1.2f;
    float bgRadius = radius * backgroundScale;
    float bgRx = centerX - bgRadius;
    float bgRy = centerY - bgRadius;
    
    if (!knobImage.isNull())
    {
        //Background
        //g.drawImageTransformed(knobImage, juce::AffineTransform::scale(scaleFactor * backgroundScale).translated(bgRx, bgRy), false);
        
        //Knob
        g.drawImageTransformed(knobImage, juce::AffineTransform::scale(scaleFactor).translated(rx, ry).rotated(angle, centerX, centerY), false);
        
    }
    else
    {
        g.drawText("Knob image missing", 0, 0, width, height, juce::Justification::centred);
    }
}
