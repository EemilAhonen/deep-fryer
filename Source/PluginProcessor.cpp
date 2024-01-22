/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    createParameter("tone", "Tone", 0.0, 100.0, 0.0);
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    // Setting the smoothing parameter for ramping value transitions in audio processing
    double smoothingParameter = 0.1;
    
    //Prepare the audio processing blocks
    gainModule.prepare(spec);
    dryWetMixerModule.prepare(spec);
    filterModule.prepare(spec);
    
    // Set the ramp duration for transitioning audio processing parameters
    gainModule.setRampDurationSeconds(smoothingParameter);
    
    
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
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Clear any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Initialize parameter pointers
    // Input and Output gain -24db to 24db
    float inputVolumeValue   = *treeState.getRawParameterValue("inputVolume");
    float outputVolumeValue  = *treeState.getRawParameterValue("outputVolume");
    
    // Dry-Wet value from 0 to 100%
    float mixValue       = *treeState.getRawParameterValue("mix");
    
    // Amount of drive 0 to 100
    float driveValue     = *treeState.getRawParameterValue("drive");
    
    // Amount of filter 0 to 100
    float filterValue    = *treeState.getRawParameterValue("clarity");
    
    // Amount of tone 0 to 100
    float toneValue      = *treeState.getRawParameterValue("tone");
    
    //Initialize an AudioBlock using the audio buffer
    juce::dsp::AudioBlock<float> block {buffer};
    
    //Set the gain in decibels based on the inputVolumeValue
    gainModule.setGainDecibels(inputVolumeValue);
    gainModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    //Set the gain in decibels based on the outputVolumeValue
    gainModule.setGainDecibels(outputVolumeValue);
    gainModule.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool DeepFryerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DeepFryerAudioProcessor::createEditor()
{
    //return new DeepFryerAudioProcessorEditor (*this);
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
