/*
  ==============================================================================

    UI-Resized.cpp
    Created: 23 Jan 2024 8:54:17pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

void DeepFryerAudioProcessorEditor::uiResized()
{
    // Ratio from my photo editing app to scale coordinates
    // Not the smartest way to go around this, especially given
    // JUCE doesn't support floating point coordinates oro pixel perfect scaling
    float ratio = getWidth() / 3000.0f;
    
    float bigKnobSize = 800.0f * ratio;
    float smallKnobSize = 497.5f * ratio;
    float smallKnobY = 442.0f * ratio;
    
    float inputVolumeX = 35.0f * ratio;
    float toneX = 567.0f * ratio;
    float clarityX = 1935.0f * ratio;
    float outputX = 2467.0f * ratio;
    
    float driveKnobX = 1100.0f * ratio;
    float driveKnobY = 290.0f * ratio;
    
    float mixSliderWidth = 340.0f * ratio;
    float mixSliderHeight = 150.0f * ratio;
    float mixSliderX = 2620.0f * ratio;
    float mixSliderY = 0.0f;
    
    inputVolumeKnob.setBounds(inputVolumeX, smallKnobY, smallKnobSize, smallKnobSize);
    outputVolumeKnob.setBounds(outputX, smallKnobY, smallKnobSize, smallKnobSize);
    driveKnob.setBounds(driveKnobX, driveKnobY, bigKnobSize, bigKnobSize);
    toneKnob.setBounds(toneX, smallKnobY, smallKnobSize, smallKnobSize);
    clarityKnob.setBounds(clarityX, smallKnobY, smallKnobSize, smallKnobSize);
    mixSlider.setBounds(mixSliderX, mixSliderY, mixSliderWidth, mixSliderHeight);
    
    savePluginBounds();
}
