/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Globals/Parameters.h"

//==============================================================================

class DeepFryerAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DeepFryerAudioProcessor();
    ~DeepFryerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //== Parameters ================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    Parameters _parameters;
    juce::AudioProcessorValueTreeState _treeState;
    
    juce::ValueTree _valueTree
    { "Variables", {},
        {
            { "Group", {{ "name", "variables" }},
                {
                    { "Parameter", {{ "id", "width" }, { "value", 0.0 }}},
                    { "Parameter", {{ "id", "width" }, { "value", 0.0 }}},
                }
                
            }
        }
    };
    
    float _width = 0.0f;
    float _height = 0.0f;
    
    //== Processing ================================================================
    void clearUnusedOutputChannels(juce::AudioBuffer<float>& buffer);
    void processDistortion(juce::dsp::AudioBlock<float>& block);
    void processTone(juce::dsp::AudioBlock<float>& block);
    
private:
    //== Parameters =================================================================
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void addParameterListeners();
    
    //== Modules ===================================================================
    // Module for changing the input volujme
    juce::dsp::Gain<float> _inputVolumeModule;
    
    // Module for changing the output volume
    juce::dsp::Gain<float> _outputVolumeModule;

    // Module for tone filter
    juce::dsp::ProcessorDuplicator <juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients <float>> _toneFilterModule;
    
    // Module for changing the processing filter
    juce::dsp::LinkwitzRileyFilter<float> _clarityModule;
    
    // Module for changing the dry-wet amount
    juce::dsp::DryWetMixer<float> _dryWetMixerModule;
    
    //== Values ====================================================================
    // User sample rate
    float _lastSampleRate;
    
    // Setting the smoothing parameter for ramping value transitions in audio processing
    const double _smoothingParameter = 0.001;
    
    // Values for user modifiable parameters
    juce::SmoothedValue<float> _inputVolumeValue; //-24db to 24db
    juce::SmoothedValue<float> _outputVolumeValue; //-24db to 24db
    juce::SmoothedValue<float> _driveValue; // 0-1
    float _toneValue = 0.f;
    juce::SmoothedValue<float> _clarityValue;
    float _mixValue = 1.f; // 0-1
    
    // Frequency and Q for tone filter
    const float _toneFrequency = 10000.f;
    const float _toneQ = 0.3f;
    
    // Maximum values for parameters
    const float _maximumDrive = 50.f; //in db
    const float _maximumTone = 24.f; //in db
    const float _maximumClarity = 1000.f; //in hz
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeepFryerAudioProcessor)
};
