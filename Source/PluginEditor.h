#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VerticalMeter.h"

class GainPlugAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    GainPlugAudioProcessorEditor (GainPlugAudioProcessor&);
    ~GainPlugAudioProcessorEditor() override;

    void timerCallback() override;
    void paint (juce::Graphics&) override;
    void resized() override;

    VerticalGradientMeter inputVUMeterLeft, inputVUMeterRight, outputVUMeterLeft, outputVUMeterRight;

private:
    juce::Slider dBGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dBGainSliderAttachment;

    GainPlugAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainPlugAudioProcessorEditor)
};