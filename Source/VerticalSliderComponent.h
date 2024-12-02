#pragma once

#ifndef VERTICALSLIDERCOMPONENT_H
#define VERTICALSLIDERCOMPONENT_H

#include <JuceHeader.h>

class VerticalSliderComponent  : public juce::Component
{
public:
    VerticalSliderComponent(juce::AudioProcessorValueTreeState& apvts, const juce::String& parameterID)
        : sliderAttachment(apvts, parameterID, slider)
    {
        slider.setSliderStyle(juce::Slider::LinearVertical);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        addAndMakeVisible(slider);
    }

    ~VerticalSliderComponent() override = default;

    void resized() override
    {
        slider.setBounds(getLocalBounds());
    }

private:
    juce::Slider slider;
    juce::AudioProcessorValueTreeState::SliderAttachment sliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VerticalSliderComponent)
};

#endif