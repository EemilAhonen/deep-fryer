/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DeepFryerAudioProcessorEditor::DeepFryerAudioProcessorEditor (DeepFryerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setResizable(true, true);
    getConstrainer() -> setFixedAspectRatio(2.0);
    setResizeLimits(500, 250, 1500, 750);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 500);
}

DeepFryerAudioProcessorEditor::~DeepFryerAudioProcessorEditor()
{
}

//==============================================================================
void DeepFryerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black.brighter(0.2));
    g.setColour (juce::Colours::white);
    g.setFont (35.0f);
}

void DeepFryerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
