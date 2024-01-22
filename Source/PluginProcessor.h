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
    void clearUnusedOutputChannels(juce::AudioBuffer<float>& buffer);
    
    /** Value Trees  ===========================================================*/
    juce::AudioProcessorValueTreeState treeState;

private:
    /** Parameters  ===========================================================*/
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    //Module for changing the input volujme
    juce::dsp::Gain<float> inputVolumeModule;
    
    //Module for changing the output volume
    juce::dsp::Gain<float> outputVolumeModule;
    
    //Module for changing the dry-wet amount
    juce::dsp::DryWetMixer<float> dryWetMixerModule;
    
    //Module for changing the processing filter
    juce::dsp::LinkwitzRileyFilter<float> filterModule;
    
    //Module for tone filter
    juce::dsp::ProcessorDuplicator <juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients <float>> toneFilter;
    
    //User sample rate
    float lastSampleRate;
    
    //Smoothed values for parameters
    juce::SmoothedValue<float> drive;
    juce::SmoothedValue<float> ceiling;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeepFryerAudioProcessor)
};
