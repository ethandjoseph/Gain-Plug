#pragma once

#ifndef VUMETERCOMPONENT_H
#define VUMETERCOMPONENT_H

#include <JuceHeader.h>

class VUMeterComponent  : public juce::Component, private juce::Timer
{
public:
    VUMeterComponent()
    {
        startTimerHz(30);
    }

    ~VUMeterComponent() override = default;

    void processBuffer(const juce::AudioBuffer<float>& buffer)
    {
        const int numChannels = buffer.getNumChannels();

        for (int channel = 0; channel < std::min(numChannels, 2); ++channel)
        {
            const float newRms = calculateRMS(buffer, channel);
            rmsLevels[channel] = smoothingFactor * rmsLevels[channel] + (1.0f - smoothingFactor) * newRms;
        }
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds();
        const int channelWidth = bounds.getWidth() / 2;

        for (int channel = 0; channel < 2; ++channel)
        {
            auto channelBounds = bounds.removeFromLeft(channelWidth);

            g.setColour(juce::Colours::black);
            g.fillRect(channelBounds);

            const int barHeight = static_cast<int>(channelBounds.getHeight() * rmsLevels[channel]);

            juce::Colour barColor = (rmsLevels[channel] >= clippingThreshold) ? juce::Colours::red : juce::Colours::yellow;
            g.setColour(barColor);

            g.fillRect(channelBounds.removeFromBottom(barHeight));
        }
    }

    void resized() override
    {
    }

private:
    float rmsLevels[2] = { 0.0f, 0.0f };
    const float smoothingFactor = 0.9f;
    const float clippingThreshold = 0.9f;

    void timerCallback() override
    {
        repaint();
    }

    float calculateRMS(const juce::AudioBuffer<float>& buffer, int channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        const int numSamples = buffer.getNumSamples();

        float sum = 0.0f;
        for (int i = 0; i < numSamples; ++i)
            sum += channelData[i] * channelData[i];

        return std::sqrt(sum / numSamples);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VUMeterComponent)
};

#endif