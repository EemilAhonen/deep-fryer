/*
  ==============================================================================

    UI-Paint.cpp
    Created: 23 Jan 2024 8:54:08pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "../PluginEditor.h"

//==============================================================================

void DeepFryerAudioProcessorEditor::uiPaint(juce::Graphics &g)
{
    // Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    
    // Load and set background image if available
    juce::Image backgroundImage = juce::ImageCache::getFromMemory(BinaryData::Background_png, BinaryData::Background_pngSize);

    if (!backgroundImage.isNull())
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    }
}
