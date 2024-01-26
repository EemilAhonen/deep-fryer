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
    uiConstructor();
    
}

DeepFryerAudioProcessorEditor::~DeepFryerAudioProcessorEditor()
{
    inputVolumeKnob.setLookAndFeel(nullptr);
    inputVolumeKnob.setLookAndFeel(nullptr);
    outputVolumeKnob.setLookAndFeel(nullptr);
    driveKnob.setLookAndFeel(nullptr);
    toneKnob.setLookAndFeel(nullptr);
    clarityKnob.setLookAndFeel(nullptr);
    mixSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void DeepFryerAudioProcessorEditor::paint (juce::Graphics& g)
{
    uiPaint(g);
}

void DeepFryerAudioProcessorEditor::resized()
{
    uiResized();
}

void DeepFryerAudioProcessorEditor::savePluginBounds()
{
    audioProcessor.valueTree.setProperty("width", getWidth(), nullptr);
    audioProcessor.valueTree.setProperty("height", getHeight(), nullptr);
    audioProcessor._width = getWidth();
    audioProcessor._height = getHeight();
}
