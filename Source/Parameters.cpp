/*
  ==============================================================================

    Parameters.cpp
    Created: 27 Jan 2024 8:37:05pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "Parameters.h"
#include "Globals.h"

Parameters::Parameters()
{
    initParameters();
}

void Parameters::initParameters()
{
    // Add your parameters here
    addParameter(inputVolumeID, inputVolumeName, -24.0f, 24.0f, 0.0f, 0.1f, 35.0f, 442.0f, 497.5f, 497.5f, 0);
    addParameter(outputVolumeID, outputVolumeName, -24.0f, 24.0f, 0.0f, 0.1f, 2467.0f, 442.0f, 497.5f, 497.5f, 0);
    addParameter(driveID, driveName, 0.0f, 100.0f, 0.0f, 0.1f, 1100.0f, 290.0f, 800.0f, 800.0f, 0);
    addParameter(toneID, toneName, -100.0f, 100.0f, 0.0f, 0.1f, 567.0f, 442.0f, 497.5f, 497.5f, 0);
    addParameter(clarityID, clarityName, 0.0f, 100.0f, 0.0f, 0.1f, 1935.0f, 497.5f, 497.5f, 497.5f, 0);
    addParameter(mixID, mixName, 0.0f, 100.0f, 100.0f, 0.1f, 2620.0f, 0.0f, 340.0f, 150.0f, 1);
}

void Parameters::addParameter(const juce::String id, const juce::String name, float minValue, float maxValue, float initValue, float interval, float x, float y, float width, float height, int lookAndFeelID)
{
    _sliderComponents.push_back(std::make_unique<SliderComponent>(id, name, minValue, maxValue, initValue, interval, x, y, width, height, lookAndFeelID));
    _audioParameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{id, 1}, name, minValue, maxValue, initValue));
}
