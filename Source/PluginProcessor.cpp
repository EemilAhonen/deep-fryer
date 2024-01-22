/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AudioHelpers.h"

//==============================================================================
DeepFryerAudioProcessor::DeepFryerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                  ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

DeepFryerAudioProcessor::~DeepFryerAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout DeepFryerAudioProcessor::createParameterLayout()
{
    //Create vector of parameters
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Create individual parameters
    auto createParameter = [&params](const char* id, const char* name, double minValue, double maxValue, double defaultValue) {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{id, 1}, name, minValue, maxValue, defaultValue));
    };
    
    createParameter("inputVolume", "Input Volume", -24.0, 24.0, 0.0);
    createParameter("outputVolume", "Output Volume", -24.0, 24.0, 0.0);
    createParameter("mix", "Mix", 0.0, 100.0, 100.0);
    createParameter("drive", "Drive", 0.0, 100.0, 0.0);
    createParameter("tone", "Tone", -100.0, 100.0, 0.0);
    createParameter("clarity", "Clarity", 0.0, 100.0, 0.0);
    
    //Return initialized parameters
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String DeepFryerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DeepFryerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DeepFryerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DeepFryerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DeepFryerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DeepFryerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int DeepFryerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DeepFryerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DeepFryerAudioProcessor::getProgramName (int index)
{
    return {};
}

void DeepFryerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DeepFryerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Store the sample rate for later use
    lastSampleRate = sampleRate;
    
    // Setup audio processing specifications
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    // Setting the smoothing parameter for ramping value transitions in audio processing
    double smoothingParameter = 0.01;
    
    //Prepare the audio processing blocks
    inputVolumeModule.prepare(spec);
    outputVolumeModule.prepare(spec);
    dryWetMixerModule.prepare(spec);
    filterModule.prepare(spec);
    
    // Prepare and reset the tone filter
    toneFilter.prepare(spec);
    toneFilter.reset();
    
    // Reset and set the ramp duration for transitioning audio processing parameters
    inputVolumeModule.setRampDurationSeconds(smoothingParameter);
    outputVolumeModule.setRampDurationSeconds(smoothingParameter);
    drive.reset(lastSampleRate, smoothingParameter);
    ceiling.reset(lastSampleRate, smoothingParameter);
}

void DeepFryerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DeepFryerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif

void DeepFryerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Clear any output channels that didn't contain input data
    clearUnusedOutputChannels(buffer);
    
    // Initialize parameter values
    // Input and Output gain from -24db to 24db
    float inputVolumeValue = *treeState.getRawParameterValue("inputVolume");
    float outputVolumeValue = *treeState.getRawParameterValue("outputVolume");
    
    // Dry-Wet value from 0 to 1
    float mixValue = *treeState.getRawParameterValue("mix") / 100;
    
    // Amount of drive from 0 to 1
    float driveValue = *treeState.getRawParameterValue("drive") / 100;
    
    // Set 0 to 24db target value to drive smoothing
    drive.setTargetValue(juce::Decibels::decibelsToGain(driveValue * 24));
    
    // Set 0.5 to 1 target value to ceiling smoothing
    ceiling.setTargetValue(1 - driveValue / 2);
    
    // Amount of filter from 0 to 100
    float filterValue = *treeState.getRawParameterValue("clarity");
    
    // Amount of tone from -25db to 25db
    float toneValue = juce::Decibels::decibelsToGain(*treeState.getRawParameterValue("tone")/ 5);
    
    // Frequency and Q for tone filter
    const float toneFrequency = 10000.0f;
    const float toneQ = 0.3f;
    
    // Initialize an AudioBlock using the audio buffer
    juce::dsp::AudioBlock<float> block {buffer};
    
    // Add dry block to dry-wet mixer
    dryWetMixerModule.pushDrySamples(block);
    
    // Set the input gain in decibels based on the inputVolumeValue
    inputVolumeModule.setGainDecibels(inputVolumeValue);
    inputVolumeModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // Distortion processing loop
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float bufferSample = channelData[sample];
            float saturatedSample = saturateSample(bufferSample, drive.getNextValue(), ceiling.getNextValue());
            channelData[sample] = saturatedSample;
        }
    }
    
    // Apply tone filter
    *toneFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, toneFrequency, toneQ, toneValue);
    toneFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // Set the output gain in decibels based on the outputVolumeValue
    outputVolumeModule.setGainDecibels(outputVolumeValue);
    outputVolumeModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // Set the balance between dry-wet mix
    dryWetMixerModule.setWetMixProportion(mixValue);
    dryWetMixerModule.mixWetSamples(block);
}

void DeepFryerAudioProcessor::clearUnusedOutputChannels(juce::AudioBuffer<float>& buffer)
{
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
}

//==============================================================================
bool DeepFryerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DeepFryerAudioProcessor::createEditor()
{
    //return new DeepFryerAudioProcessorEditor (*this);
    //Generic UI for debugging
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void DeepFryerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DeepFryerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DeepFryerAudioProcessor();
}
