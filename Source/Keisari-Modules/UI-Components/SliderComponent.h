/*
  ==============================================================================

    SliderComponent.h
    Created: 27 Jan 2024 3:27:03am
    Author:  Eemil Ahonen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================

// TODO: Make an elder class and inherit from that, this is only for sliders now

class SliderComponent
{
public:
    SliderComponent(const juce::String id, const juce::String name, const float minValue, const float maxValue, const float initValue, const float interval, const float x, const float y, const float width, const float height, int lookAndFeelID)
        : _id(id), _name(name), _minValue(minValue), _maxValue(maxValue), _initValue(initValue), _interval(interval), _x(x), _y(y), _width(width), _height(height), _lookAndFeelID(lookAndFeelID)
    {
        
    }

    void setBounds(float ratio = 1.0f);
    juce::Slider& getSlider() { return _slider; }
    
public:
    const juce::String _id;
    const juce::String _name;
    
    const float _minValue;
    const float _maxValue;
    const float _initValue;
    const float _interval;
    
    const float _x;
    const float _y;
    const float _width;
    const float _height;
    
    const int _lookAndFeelID;
    
private:
    juce::Slider _slider;
};
