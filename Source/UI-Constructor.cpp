/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 23 Jan 2024 8:53:51pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

void DeepFryerAudioProcessorEditor::createKnob(juce::Slider& knob, const juce::String& parameterID, float minValue, float maxValue, float interval, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment)
{
    addAndMakeVisible(knob);
    knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 55, 25);
    knob.setDoubleClickReturnValue(true, 0.0);
    knob.setRange(minValue, maxValue, interval);
    knob.setLookAndFeel(&customKnob);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, parameterID, knob);
}

void DeepFryerAudioProcessorEditor::uiConstructor()
{
    // TODO: REDO THIS
    
    // Create UI knobs
    createKnob(inputVolumeKnob, inputVolumeID, -24.0f, 24.0f, 0.01f, inputVolumeKnobAttachment);
    createKnob(outputVolumeKnob, outputVolumeID, -24.0f, 24.0f, 0.01f, outputVolumeKnobAttachment);
    createKnob(driveKnob, driveID, -100.0f, 100.0f, 0.01f, driveKnobAttachment);
    createKnob(toneKnob, toneID, 0.0f, 100.0f, 0.01f, toneKnobAttachment);
    createKnob(clarityKnob, clarityID, 0.0f, 100.0f, 0.01f, clarityKnobAttachment);
    
    // Create mix slider
    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 55, 25);
    mixSlider.setDoubleClickReturnValue(true, 0.0);
    mixSlider.setRange(0.0f, 100.0f, 0.01f);
    mixSlider.setLookAndFeel(&simpleSlider);
    mixSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, mixID, mixSlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    if (audioProcessor._width == 0.0)
    {
        setSize (INIT_WIDTH, INIT_HEIGHT);
    }
    else
    {
        DBG(audioProcessor._width);
        setSize(audioProcessor._width, audioProcessor._height);
    }
    
    // Set resizable and contraints
    setResizable(true, true);
    getConstrainer() -> setFixedAspectRatio(2.0);
    setResizeLimits(MIN_WIDTH, MIN_HEIGHT, MAX_WIDTH, MAX_HEIGHT);
}
