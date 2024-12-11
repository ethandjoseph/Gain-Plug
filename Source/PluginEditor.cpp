#include "PluginProcessor.h"
#include "PluginEditor.h"

GainPlugAudioProcessorEditor::GainPlugAudioProcessorEditor (GainPlugAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    dBGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    dBGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    addAndMakeVisible(inputVUMeter);
    addAndMakeVisible(dBGainSlider);
    addAndMakeVisible(outputVUMeter);

    dBGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN_DB", dBGainSlider);
    
    setSize (200, 300);
}

GainPlugAudioProcessorEditor::~GainPlugAudioProcessorEditor()
{
}

void GainPlugAudioProcessorEditor::paint (juce::Graphics& g)
{
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);
}

void GainPlugAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    const int vuMeterWidth = 50;
    const int sliderWidth = 100;
    const int vuMeterInset = 10;

    inputVUMeter.setBounds(bounds.removeFromLeft(vuMeterWidth).reduced(vuMeterInset));
    outputVUMeter.setBounds(bounds.removeFromRight(vuMeterWidth).reduced(vuMeterInset));
    dBGainSlider.setBounds(bounds.removeFromLeft(sliderWidth));
}