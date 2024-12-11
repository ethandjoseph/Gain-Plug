#pragma once

#ifndef GAIN_H
#define GAIN_H

#include <JuceHeader.h>

class GainProcessor
{
public:
    GainProcessor() : gain(1.0f) {}
    virtual ~GainProcessor() = default;

    void setGain(float gainValue) { gain = juce::jlimit(0.0f, 2.0f, gainValue); }
    void process(juce::AudioBuffer<float>& buffer);

private:
    float gain;
};

#endif