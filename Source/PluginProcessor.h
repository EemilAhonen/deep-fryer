/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class DeepFryerAudioProcessor  : public juce::AudioProcessor
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

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void clearUnusedOutputChannels(juce::AudioBuffer<float>& buffer);
    void updateParameters();
    
    /** Value Trees  ===========================================================*/
    juce::AudioProcessorValueTreeState treeState;

private:
    /** Parameters  ===========================================================*/
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    // Module for changing the input volujme
    juce::dsp::Gain<float> inputVolumeModule;
    
    // Module for changing the output volume
    juce::dsp::Gain<float> outputVolumeModule;

    // Module for tone filter
    juce::dsp::ProcessorDuplicator <juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients <float>> toneFilter;
    
    // Module for changing the processing filter
    juce::dsp::LinkwitzRileyFilter<float> clarityModule;
    
    // Module for changing the dry-wet amount
    juce::dsp::DryWetMixer<float> dryWetMixerModule;
    
    // User sample rate
    float lastSampleRate;
    
    // Values for user modifiable parameters
    juce::SmoothedValue<float> inputVolumeValue;
    juce::SmoothedValue<float> outputVolumeValue;
    juce::SmoothedValue<float> driveValue;
    float toneValue = 0.f; // in db
    float clarityValue = 20.0f; // in hz
    float mixValue = 1.f; // 0-1
    
    // Frequency and Q for tone filter
    const float toneFrequency = 10000.0f;
    const float toneQ = 0.3f;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeepFryerAudioProcessor)
};
