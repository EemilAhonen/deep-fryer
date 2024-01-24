/*
  ==============================================================================

    UI-Paint.cpp
    Created: 23 Jan 2024 8:54:08pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

void DeepFryerAudioProcessorEditor::uiPaint(juce::Graphics &g)
{
    // Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    
    // Set background if image is found
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::Background_png, BinaryData::Background_pngSize);

    if (!backgroundImage.isNull())
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    }
    
    // We don't use these in this plugin anyways
    g.setColour (juce::Colours::white);
    g.setFont (50.0f);
}
