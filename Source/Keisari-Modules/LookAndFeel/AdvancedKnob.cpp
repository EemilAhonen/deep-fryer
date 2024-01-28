/*
  ==============================================================================

    AdvancedKnob.cpp
    Created: 23 Jan 2024 9:42:00pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "AdvancedKnob.h"

//==============================================================================

AdvancedKnob::AdvancedKnob()
{
    
}

void AdvancedKnob::setParams(juce::Image knobImage)
{
    _knobImage = knobImage;
}

void AdvancedKnob::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPosition, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
    float diameter = juce::jmin(width, height);
    float radius = diameter / 2;
    float angle = rotaryStartAngle + (sliderPosition * (rotaryEndAngle - rotaryStartAngle));
    auto center = bounds.getCentre();

    // Draw knob image if it's set
    if (!_knobImage.isNull())
    {
        // Calculate the transformation for the knob image
        auto transform = juce::AffineTransform::scale(diameter * _innerScale / _knobImage.getHeight()).translated(center.x - radius * _innerScale, center.y - radius * _innerScale).rotated(angle, center.x, center.y);
        g.drawImageTransformed(_knobImage, transform, false);
    }

    // Calculate outline properties
    float outlineW = radius * (1 - _innerScale);
    float outlineOffset = radius * _outlineWidth;

    // Function to draw an arc
    auto drawArc = [&g, &bounds, &rotaryStartAngle, &rotaryEndAngle](const juce::Colour& color, float lineWidth, float innerRadius, float outerRadius) {
        juce::Path arc;
        arc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), outerRadius, outerRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);

        g.setColour(color);
        g.strokePath(arc, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    };

    // Draw the outline arc
    drawArc(_outlineColor, outlineW, radius, radius - outlineW * 0.5f);

    // Calculate line width and arc radius for background arc
    float lineW = outlineW - outlineOffset * 2;
    float arcRadius = radius - lineW * 0.5f - outlineOffset;

    // Draw the background arc
    drawArc(_backgroundColor, lineW, arcRadius, arcRadius);

    // Draw the value arc if the slider is enabled
    if (slider.isEnabled())
    {
        juce::Path fillArc;
        fillArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, rotaryStartAngle, angle, true);

        g.setColour(_fillColor);
        g.strokePath(fillArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    // Calculate thumb position and draw the thumb
    juce::Point<float> thumbPoint(center.x + arcRadius * std::cos(angle - juce::MathConstants<float>::halfPi), center.y + arcRadius * std::sin(angle - juce::MathConstants<float>::halfPi));
    
    g.setColour(_thumbColor);
    g.fillEllipse(juce::Rectangle<float>(outlineW, outlineW).withCentre(thumbPoint));
}
