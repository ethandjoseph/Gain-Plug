#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Gain.h"

juce::AudioProcessorValueTreeState::ParameterLayout GainPlugAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "dBGain",             // Parameter ID
        "Gain",               // Parameter name
        -60.0f,               // Min value in dB
        0.0f,                 // Max value in dB
        -12.0f                // Default value in dB
    ));

    return { params.begin(), params.end() };
}

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
    gainProcessor.setGain(1.0f);
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

    if (auto* editor = dynamic_cast<GainPlugAudioProcessorEditor*>(getActiveEditor()))
    {
        editor->inputVUMeter.processBuffer(buffer);
    }

    float dBValue = apvts.getRawParameterValue("dBGain")->load();
    float linearGain = juce::Decibels::decibelsToGain(dBValue);
    gainProcessor.setGain(linearGain);
    gainProcessor.process(buffer);

    if (auto* editor = dynamic_cast<GainPlugAudioProcessorEditor*>(getActiveEditor()))
    {
        editor->outputVUMeter.processBuffer(buffer);
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

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainPlugAudioProcessor();
}