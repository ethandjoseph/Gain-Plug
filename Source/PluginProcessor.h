#pragma once
#include <JuceHeader.h>
#include "Gain.h"

class GainPlugAudioProcessor  : public juce::AudioProcessor
{
public:
    GainPlugAudioProcessor();
    ~GainPlugAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

    float getRMSValue(const int bus, const int channel) const;

private:
    GainProcessor gainProcessor;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::LinearSmoothedValue<float> inputRMSLevelLeft, inputRMSLevelRight, outputRMSLevelLeft, outputRMSLevelRight;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainPlugAudioProcessor)
};