/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "HelperFunctions.h"

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
    treeState.addParameterListener(inputVolumeID, this);
    treeState.addParameterListener(outputVolumeID, this);
    treeState.addParameterListener(driveID, this);
    treeState.addParameterListener(toneID, this);
    treeState.addParameterListener(clarityID, this);
    treeState.addParameterListener(mixID, this);
}

DeepFryerAudioProcessor::~DeepFryerAudioProcessor()
{
    treeState.addParameterListener(inputVolumeID, this);
    treeState.addParameterListener(outputVolumeID, this);
    treeState.addParameterListener(driveID, this);
    treeState.addParameterListener(toneID, this);
    treeState.addParameterListener(clarityID, this);
    treeState.addParameterListener(mixID, this);
}

//== Parameter functions =======================================================

juce::AudioProcessorValueTreeState::ParameterLayout DeepFryerAudioProcessor::createParameterLayout()
{
    //Create vector of parameters
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{inputVolumeID, 1}, inputVolumeName, -24.0f, 24.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{outputVolumeID, 1}, outputVolumeName, -24.0f, 24.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{driveID, 1}, driveName, 0.0f, 100.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{toneID, 1}, toneName, -100.0f, 100.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{clarityID, 1}, clarityName, 0.0f, 100.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{mixID, 1}, mixName, 0.0f, 100.0f, 100.0f));
    
    //Return initialized parameters
    return { params.begin(), params.end() };
}

void DeepFryerAudioProcessor::parameterChanged(const juce::String &parameterId, float newValue)
{
    //TODO: Parameter specific updating
    updateParameters();
}

void DeepFryerAudioProcessor::updateParameters()
{
    // Update parameter values
    inputVolumeValue.setTargetValue(*treeState.getRawParameterValue(inputVolumeID));
    outputVolumeValue.setTargetValue(*treeState.getRawParameterValue(outputVolumeID));
    
    // Scale the drive parameter from 0-100 to 0-1
    driveValue.setTargetValue(*treeState.getRawParameterValue(driveID) / 100);
    
    // Amount of tone from -maximumTone to maximumTone
    toneValue = juce::Decibels::decibelsToGain(*treeState.getRawParameterValue(toneID) / 100 * maximumTone);
    
    // Scale the clarity parameter from 0 to 100 to a cutoff frequency from 20 to maximumClarity
    clarityValue.setTargetValue(max(*treeState.getRawParameterValue(clarityID), 20.0f));
    clarityModule.setCutoffFrequency(clarityValue.getNextValue() * maximumClarity / 10);
    
    // Scale the mix parameter from 0-100 to 0-1
    mixValue = *treeState.getRawParameterValue(mixID) / 100;
}

//== Processing functions ======================================================

void DeepFryerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Store the sample rate for later use
    lastSampleRate = sampleRate;
    
    // Setup audio processing specifications
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    //Prepare the audio processing blocks
    inputVolumeModule.prepare(spec);
    outputVolumeModule.prepare(spec);
    toneFilter.prepare(spec);
    dryWetMixerModule.prepare(spec);
    clarityModule.prepare(spec);
    
    // Reset and set the ramp duration for transitioning audio processing parameters
    inputVolumeValue.reset(lastSampleRate, smoothingParameter);
    outputVolumeValue.reset(lastSampleRate, smoothingParameter);
    driveValue.reset(lastSampleRate, smoothingParameter * 40);
    clarityValue.reset(lastSampleRate, smoothingParameter * 100);
    toneFilter.reset();
    
    updateParameters();
}

void DeepFryerAudioProcessor::clearUnusedOutputChannels(juce::AudioBuffer<float>& buffer)
{
    // Get the total number of input and output channels
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
}

void DeepFryerAudioProcessor::processDistortion(juce::dsp::AudioBlock<float>& block)
{
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float drive = driveValue.getNextValue();
            
            // How much clarity effects ceiling. 0.3 - 1 with / 500
            float clarityFactor = clarityValue.getNextValue() / 500;
            
            // Calculate ceiling and compensation value for clipping
            float ceiling = 1 - drive * 0.5f - clarityFactor;
            float gainCompensation = 1 + drive;
            
            // Apply distortion
            float saturatedSample = channelData[sample] * juce::Decibels::decibelsToGain(drive * maximumDrive);
            float outputLow, outputHigh;
            
            // Split signal into 2 bands
            clarityModule.processSample(channel, saturatedSample, outputLow, outputHigh);
            
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
    *toneFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, toneFrequency, toneQ, toneValue);
    toneFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void DeepFryerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Clear any output channels that didn't contain input data
    clearUnusedOutputChannels(buffer);
    
    // Initialize an AudioBlock using the audio buffer
    juce::dsp::AudioBlock<float> block {buffer};
    
    // Add dry block to dry-wet mixer
    dryWetMixerModule.pushDrySamples(block);
    
    // Set the input volume in decibels based on the inputVolumeValue
    inputVolumeModule.setGainDecibels(inputVolumeValue.getNextValue());
    inputVolumeModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // Process distortion
    processDistortion(block);
    
    // Process tone filter
    processTone(block);
    
    // Set the output gain in decibels based on the outputVolumeValue
    outputVolumeModule.setGainDecibels(outputVolumeValue.getNextValue());
    outputVolumeModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // Set the balance between dry-wet mix
    dryWetMixerModule.setWetMixProportion(mixValue);
    dryWetMixerModule.mixWetSamples(block);
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

//==============================================================================
bool DeepFryerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DeepFryerAudioProcessor::createEditor()
{
    return new DeepFryerAudioProcessorEditor (*this);
    //Generic UI for debugging
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void DeepFryerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    treeState.state.appendChild(valueTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void DeepFryerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    valueTree = tree.getChildWithName("Variables");
    
    if (tree.isValid())
    {
        treeState.state = tree;
        _width = valueTree.getProperty("width");
        _height = valueTree.getProperty("height");
        updateParameters();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DeepFryerAudioProcessor();
}
