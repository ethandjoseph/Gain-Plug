#pragma once

#ifndef GAIN_H
#define GAIN_H

#include <JuceHeader.h>

class GainProcessor
{
public:
    GainProcessor() {}

    void setGain(float gainValue) { gain = gainValue; }
    void process(juce::AudioBuffer<float>& buffer);

private:
    float gain = 1.0f;
};

#endif