#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Gain.h"

GainPlugAudioProcessor::GainPlugAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

GainPlugAudioProcessor::~GainPlugAudioProcessor()
{
}

const juce::String GainPlugAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainPlugAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainPlugAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainPlugAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainPlugAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainPlugAudioProcessor::getNumPrograms()
{
    return 1;
}

int GainPlugAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainPlugAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GainPlugAudioProcessor::getProgramName (int index)
{
    return {};
}

void GainPlugAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void GainPlugAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //gainProcessor.setGain(0.5f);
    inputRMSLevelLeft.reset(sampleRate, 0.5); // 0.5 = rampLength in seconds
    inputRMSLevelRight.reset(sampleRate, 0.5);

    inputRMSLevelLeft.setCurrentAndTargetValue(-100.f);
    inputRMSLevelRight.setCurrentAndTargetValue(-100.f);

    outputRMSLevelLeft.reset(sampleRate, 0.5); // 0.5 = rampLength in seconds
    outputRMSLevelRight.reset(sampleRate, 0.5);

    outputRMSLevelLeft.setCurrentAndTargetValue(-100.f);
    outputRMSLevelRight.setCurrentAndTargetValue(-100.f);
}

void GainPlugAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainPlugAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo()
        || layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}
#endif

// Process Block ===========================================================================================//
void GainPlugAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    inputRMSLevelLeft.skip(buffer.getNumSamples());
    inputRMSLevelRight.skip(buffer.getNumSamples());
    {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (value < inputRMSLevelLeft.getCurrentValue())
            inputRMSLevelLeft.setTargetValue(value);
        else
            inputRMSLevelLeft.setCurrentAndTargetValue(value);
    }

    {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
        if (value < inputRMSLevelRight.getCurrentValue())
            inputRMSLevelRight.setTargetValue(value);
        else
            inputRMSLevelRight.setCurrentAndTargetValue(value);
    }

    /*inputRMSLevelLeft = juce::Decibels::gainToDecibals(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    inputRMSLevelRight = juce::Decibels::gainToDecibals(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
    outputRMSLevelLeft = juce::Decibels::gainToDecibals(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    outputRMSLevelRight = juce::Decibels::gainToDecibals(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));*/

    float dBValue = apvts.getRawParameterValue("GAIN_DB")->load();

    float linearGain = juce::Decibels::decibelsToGain(dBValue);
    gainProcessor.setGain(linearGain);

    gainProcessor.process(buffer);

    outputRMSLevelLeft.skip(buffer.getNumSamples());
    outputRMSLevelRight.skip(buffer.getNumSamples());
    {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (value < outputRMSLevelLeft.getCurrentValue())
            outputRMSLevelLeft.setTargetValue(value);
        else
            outputRMSLevelLeft.setCurrentAndTargetValue(value);
    }

    {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
        if (value < outputRMSLevelRight.getCurrentValue())
            outputRMSLevelRight.setTargetValue(value);
        else
            outputRMSLevelRight.setCurrentAndTargetValue(value);
    }
}
//==========================================================================================================//

bool GainPlugAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* GainPlugAudioProcessor::createEditor()
{
    return new GainPlugAudioProcessorEditor (*this);
}

void GainPlugAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void GainPlugAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

float GainPlugAudioProcessor::getRMSValue(const int bus, const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (bus == 0 && channel == 0)
        return inputRMSLevelLeft.getCurrentValue();
    if (bus == 0 && channel == 1)
        return inputRMSLevelRight.getCurrentValue();
    if (bus == 1 && channel == 0)
        return outputRMSLevelLeft.getCurrentValue();
    if (bus == 1 && channel == 1)
        return outputRMSLevelRight.getCurrentValue();
}

juce::AudioProcessorValueTreeState::ParameterLayout GainPlugAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "GAIN_DB",            // Parameter ID
        "Gain",               // Parameter name
        // juce::NormalisableRange<float>(-60.0f, 0.0f, 0.01f), // Range: -60 to 0 dB
        -60.0f,               // minValue
        6.0f,                 // maxValue
        -12.0f                // Default value in dB
    ));

    return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainPlugAudioProcessor();
}