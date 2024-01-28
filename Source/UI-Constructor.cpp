/*
  ==============================================================================

    UI-Constructor.cpp
    Created: 23 Jan 2024 8:53:51pm
    Author:  Eemil Ahonen

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================

void DeepFryerAudioProcessorEditor::createSlider(std::unique_ptr<SliderComponent>& sliderComponent)
{
    juce::Slider& slider = sliderComponent->getSlider();
    
    addAndMakeVisible(slider);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    slider.setDoubleClickReturnValue(true, sliderComponent->_initValue);
    slider.setRange(sliderComponent->_minValue, sliderComponent->_maxValue, sliderComponent->_interval);
    
    // Add look and feels here
    switch (sliderComponent->_lookAndFeelID)
    {
        case 0:
            slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
            slider.setLookAndFeel(&_advancedKnob);
            break;

        case 1:
            slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
            slider.setLookAndFeel(&_simpleSlider);
            break;

        // Add more cases if needed

        default:
            // Handle unknown lookAndFeelID
            break;
    }
    
    // Add slider attachment
    _sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor._treeState, sliderComponent->_id, slider));
}

void DeepFryerAudioProcessorEditor::uiConstructor()
{
    // Set advanced knob image
    juce::Image knobImage = juce::ImageCache::getFromMemory(BinaryData::Knob_Front_png, BinaryData::Knob_Front_pngSize);
    _advancedKnob.setParams(knobImage);
    
    // Create sliders
    for (auto& sliderComponent : audioProcessor._parameters.getSliderComponents())
    {
       createSlider(sliderComponent);
    }

    // Set editor size
    setSize(audioProcessor._width == 0.0 ? INIT_WIDTH : audioProcessor._width, audioProcessor._height);
        
    // Set resizable and constraints
    setResizable(true, true);
    getConstrainer()->setFixedAspectRatio(2.0);
    setResizeLimits(MIN_WIDTH, MIN_HEIGHT, MAX_WIDTH, MAX_HEIGHT);
}
