/*
  ==============================================================================

    Pro-Prepare.cpp
    Created: 27 Jan 2024 8:07:40pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "PluginProcessor.h"

void DeepFryerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Store the sample rate for later use
    _lastSampleRate = sampleRate;
    
    // Setup audio processing specifications
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    //Prepare the audio processing blocks
    _inputVolumeModule.prepare(spec);
    _outputVolumeModule.prepare(spec);
    _toneFilterModule.prepare(spec);
    _dryWetMixerModule.prepare(spec);
    _clarityModule.prepare(spec);
    
    // Reset and set the ramp duration for transitioning audio processing parameters
    _inputVolumeValue.reset(_lastSampleRate, _smoothingParameter);
    _outputVolumeValue.reset(_lastSampleRate, _smoothingParameter);
    _driveValue.reset(_lastSampleRate, _smoothingParameter * 40);
    _clarityValue.reset(_lastSampleRate, _smoothingParameter * 100);
    _toneFilterModule.reset();
    
    // Update all parameter values
    parameterChanged(juce::String(), 0.0f);
}
