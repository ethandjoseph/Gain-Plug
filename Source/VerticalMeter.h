// Credits -> https://github.com/Thrifleganger/level-meter-demo/blob/master/Source/Component/VerticalGradientMeter.h

#pragma once
#include <JuceHeader.h>

class VerticalGradientMeter : public juce::Component
{
public:
    VerticalGradientMeter() {};

    void setLevel(float rmsLevel)
    {
        level = rmsLevel;
    }

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().reduced(2);

        g.setColour(juce::Colours::black);
        g.fillRect(bounds);

        g.setGradientFill(gradient);
        const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float>(getHeight()));
        g.fillRect(bounds.removeFromBottom(scaledY));
    }

    void resized() override
    {
        const auto bounds = getLocalBounds().toFloat();
        gradient = juce::ColourGradient{
            juce::Colours::green, bounds.getBottomLeft(),
            juce::Colours::red, bounds.getTopLeft(),
            false };
        gradient.addColour(0.5, juce::Colours::yellow);
    }

private:
    float level;
    juce::ColourGradient gradient{};
};