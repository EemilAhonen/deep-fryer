/*
  ==============================================================================

    Pro-Process.cpp
    Created: 27 Jan 2024 8:07:32pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "../PluginProcessor.h"
#include "../Keisari-Modules/Helper-Functions/HelperFunctions.h"

void DeepFryerAudioProcessor::processDistortion(juce::dsp::AudioBlock<float>& block)
{
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float drive = _driveValue.getNextValue();
            
            // How much clarity effects ceiling. 0.3 - 1 with / 500
            float clarityFactor = _clarityValue.getNextValue() / 500;
            
            // Calculate ceiling and compensation value for clipping
            float ceiling = 1 - drive * 0.5f - clarityFactor;
            float gainCompensation = 1 + drive;
            
            // Apply distortion
            float saturatedSample = channelData[sample] * juce::Decibels::decibelsToGain(drive * _maximumDrive);
            float outputLow, outputHigh;
            
            // Split signal into 2 bands
            _clarityModule.processSample(channel, saturatedSample, outputLow, outputHigh);
            
            // Clamp values
            float saturatedHigh = clamp(outputHigh, -ceiling, ceiling);
            float saturatedLow = clamp(outputLow, -1.0f, 1.0f);
            
            // Update channel data
            channelData[sample] = saturatedHigh + (saturatedLow / gainCompensation);
        }
    }
}

void DeepFryerAudioProcessor::processTone(juce::dsp::AudioBlock<float>& block)
{
    // Apply tone filter
    *_toneFilterModule.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(_lastSampleRate, _toneFrequency, _toneQ, _toneValue);
    _toneFilterModule.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void DeepFryerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Clear any output channels that didn't contain input data
    clearUnusedOutputChannels(buffer);
    
    // Initialize an AudioBlock using the audio buffer
    juce::dsp::AudioBlock<float> block {buffer};
    
    // Add dry block to dry-wet mixer
    _dryWetMixerModule.pushDrySamples(block);
    
    // Set the input volume in decibels based on the inputVolumeValue
    _inputVolumeModule.setGainDecibels(_inputVolumeValue.getNextValue());
    _inputVolumeModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // Process distortion
    processDistortion(block);
    
    // Process tone filter
    processTone(block);
    
    // Set the output gain in decibels based on the outputVolumeValue
    _outputVolumeModule.setGainDecibels(_outputVolumeValue.getNextValue());
    _outputVolumeModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // Set the balance between dry-wet mix
    _dryWetMixerModule.setWetMixProportion(_mixValue);
    _dryWetMixerModule.mixWetSamples(block);
}
