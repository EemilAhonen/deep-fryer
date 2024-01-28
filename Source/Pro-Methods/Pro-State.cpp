/*
  ==============================================================================

    Pro-State.cpp
    Created: 27 Jan 2024 8:09:01pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "../PluginProcessor.h"

void DeepFryerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    _treeState.state.appendChild(_valueTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    _treeState.state.writeToStream (stream);
}

void DeepFryerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    _valueTree = tree.getChildWithName("Variables");
    
    if (tree.isValid())
    {
        _treeState.state = tree;
        _width = _valueTree.getProperty("width");
        _height = _valueTree.getProperty("height");
        
        parameterChanged(juce::String(), 0.0f);
    }
}
