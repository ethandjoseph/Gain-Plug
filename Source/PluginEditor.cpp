#include "PluginProcessor.h"
#include "PluginEditor.h"

GainPlugAudioProcessorEditor::GainPlugAudioProcessorEditor (GainPlugAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    dBGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    dBGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);

    addAndMakeVisible(inputVUMeterLeft);
    addAndMakeVisible(inputVUMeterRight);

    addAndMakeVisible(dBGainSlider);

    addAndMakeVisible(outputVUMeterLeft);
    addAndMakeVisible(outputVUMeterRight);

    dBGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN_DB", dBGainSlider);
    
    startTimerHz(30);

    setSize (200, 300);
}

GainPlugAudioProcessorEditor::~GainPlugAudioProcessorEditor()
{
}

void GainPlugAudioProcessorEditor::timerCallback()
{
    float inputLevelLeft = audioProcessor.getRMSValue(0, 0);
    float inputLevelRight = audioProcessor.getRMSValue(0, 1);
    float outputLevelLeft = audioProcessor.getRMSValue(1, 0);
    float outputLevelRight = audioProcessor.getRMSValue(1, 1);

    inputVUMeterLeft.setLevel(inputLevelLeft);
    inputVUMeterRight.setLevel(inputLevelRight);
    outputVUMeterLeft.setLevel(outputLevelLeft);
    outputVUMeterRight.setLevel(outputLevelRight);

    inputVUMeterLeft.repaint();
    inputVUMeterRight.repaint();
    outputVUMeterLeft.repaint();
    outputVUMeterRight.repaint();
}

//========================================================================================
void GainPlugAudioProcessorEditor::paint (juce::Graphics& g)
{
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::darkgrey);
}

void GainPlugAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    const int meterChannelWidth = 25;
    const int totalMeterWidth = 50;
    const int sliderWidth = 100;

    inputVUMeterLeft.setBounds(bounds.removeFromLeft(meterChannelWidth));
    inputVUMeterRight.setBounds(bounds.removeFromLeft(meterChannelWidth));

    dBGainSlider.setBounds(bounds.removeFromLeft(sliderWidth));

    outputVUMeterLeft.setBounds(bounds.removeFromLeft(meterChannelWidth));
    outputVUMeterRight.setBounds(bounds.removeFromLeft(meterChannelWidth));

}