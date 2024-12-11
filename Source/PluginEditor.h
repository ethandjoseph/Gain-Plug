#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VUMeterComponent.h"

class GainPlugAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GainPlugAudioProcessorEditor (GainPlugAudioProcessor&);
    ~GainPlugAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    VUMeterComponent inputVUMeter;
    VUMeterComponent outputVUMeter;

private:
    juce::Slider dBGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dBGainSliderAttachment;

    GainPlugAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainPlugAudioProcessorEditor)
};