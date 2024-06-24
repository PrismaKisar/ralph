#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"


RalphAudioProcessor::RalphAudioProcessor() :
    parameters(*this, nullptr, "PARAMS", Parameters::createParameterLayout()),
    drywetter(Parameters::defaultDryWet),
    downSample(44100, 44100),
    bitCrush(24, 24),
    lfoDS(Parameters::defaultFreq, Parameters::defaultWaveform),
    lfoBC(Parameters::defaultFreq, Parameters::defaultWaveform),
    DSModulation(Parameters::defaultSampleRate, Parameters::defaultAmount),
    BCModulation(Parameters::defaultBitDepth, Parameters::defaultAmount)
{
    Parameters::addListenerToAllParameters(parameters, this);
}

RalphAudioProcessor::~RalphAudioProcessor() {}

void RalphAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    drywetter.prepareToPlay(sampleRate, samplesPerBlock);
    lfoDS.prepareToPlay(sampleRate);
    lfoBC.prepareToPlay(sampleRate);
    modulation.setSize(2, samplesPerBlock);
    DSModulation.prepareToPlay(sampleRate);
    BCModulation.prepareToPlay(sampleRate);
}

void RalphAudioProcessor::releaseResources()
{
    drywetter.releaseResources();
    modulation.setSize(0, 0);
}

void RalphAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const auto numSamples = buffer.getNumSamples();
    const auto numCh = buffer.getNumChannels();
    
    lfoDS.getNextAudioBlock(modulation, numSamples);
    lfoBC.getNextAudioBlock(modulation, numSamples);
    
    DSModulation.processBlock(modulation, numSamples);
    BCModulation.processBlock(modulation, numSamples);

    for (int ch = 0; ch < numCh; ++ch)
        FloatVectorOperations::min(modulation.getWritePointer(ch), modulation.getWritePointer(ch), Parameters::maxDelayTime, numSamples);
    
    drywetter.copyDrySignal(buffer);

    downSample.processBlock(buffer);
    bitCrush.processBlock(buffer);
    
    drywetter.mixDrySignal(buffer);
}

void RalphAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == Parameters::nameDryWet) drywetter.setDWRatio(newValue);
    if (paramID == Parameters::nameFreqDS) lfoDS.setFrequency(newValue);
    if (paramID == Parameters::nameFreqBC) lfoBC.setFrequency(newValue);
    if (paramID == Parameters::nameWaveformDS) lfoDS.setWaveform(roundToInt(newValue));
    if (paramID == Parameters::nameWaveformBC) lfoBC.setWaveform(roundToInt(newValue));
    if (paramID == Parameters::nameDownSample) downSample.setSR(newValue);
    if (paramID == Parameters::nameBitCrush) bitCrush.setBits(newValue);
}


juce::AudioProcessorEditor* RalphAudioProcessor::createEditor()
{
    return new RalphAudioProcessorEditor(*this);
}

void RalphAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RalphAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
        if (xmlState.get() != nullptr)
            if (xmlState->hasTagName(parameters.state.getType()))
                parameters.replaceState(ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RalphAudioProcessor();
}
