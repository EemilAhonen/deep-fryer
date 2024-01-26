/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomKnob.h"
#include "SimpleSlider.h"

//==============================================================================
/**
*/
class DeepFryerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DeepFryerAudioProcessorEditor (DeepFryerAudioProcessor&);
    ~DeepFryerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DeepFryerAudioProcessor& audioProcessor;
    
    //== UI Modules ================================================================
    
    void uiConstructor();
    void createKnob(juce::Slider& knob, const juce::String& parameterID, float minValue, float maxValue, float interval, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment);
    void uiPaint(juce::Graphics &g);
    void uiResized();
    
    //== Parameters ================================================================
    
    const int INIT_WIDTH = 750;
    const int INIT_HEIGHT = 375;
    
    const int MIN_WIDTH = 500;
    const int MIN_HEIGHT = 250;
    
    const int MAX_HEIGHT = 1500;
    const int MAX_WIDTH = 3000;
    
    void savePluginBounds();
    
    //== Widgets ===================================================================
    
    CustomKnob customKnob;
    SimpleSlider simpleSlider;
    
    //== Parameters ================================================================
    
    juce::Slider inputVolumeKnob;
    juce::Slider outputVolumeKnob;
    juce::Slider driveKnob;
    juce::Slider toneKnob;
    juce::Slider clarityKnob;
    juce::Slider mixSlider;
    
    //== Slider listeners ==========================================================
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputVolumeKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputVolumeKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> clarityKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachment;
    
    //== Images ====================================================================
    
    juce::Image backgroundImage;
    
    // TODO: KNOB IMAGE HERE
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeepFryerAudioProcessorEditor)
};
