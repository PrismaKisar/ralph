#include "Parameters.h"

namespace Parameters {

    // PARAM IDs
    const juce::String nameGainIn = "GIN";
    const juce::String nameGainOut = "GOUT";
    const juce::String nameDryWetBC = "DWBC";
    const juce::String nameFreqBC = "MFBC";
    const juce::String nameAmountBC = "ABC";
    const juce::String nameWaveformBC = "MWBC";
    const juce::String nameBitCrush = "BC";
    const juce::String nameDryWetDS = "DWDS";
    const juce::String nameFreqDS = "MFDS";
    const juce::String nameAmountDS = "ADS";
    const juce::String nameWaveformDS = "MWDS";
    const juce::String nameDownSample = "DS";

    // Helper function to create float parameters
    std::unique_ptr<juce::RangedAudioParameter> createFloatParameter(const juce::String& id, const juce::String& name, float minValue, float maxValue, float defaultValue, float step, float skew) {
        return std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(id, 1), name, juce::NormalisableRange<float>(minValue, maxValue, step, skew), defaultValue);
    }

    // Helper function to create choice parameters
    std::unique_ptr<juce::RangedAudioParameter> createChoiceParameter(const juce::String& id, const juce::String& name, const juce::StringArray& choices, int defaultChoice) {
        return std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(id, 1), name, choices, defaultChoice);
    }

    // Create parameter layout
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

        parameters.push_back(createFloatParameter(nameGainIn, "Gain IN", minGain, maxGain, defaultGain, 0.1f, 3.0f));
        parameters.push_back(createFloatParameter(nameDryWetDS, "Dry/Wet DS (%) ", 0.0f, 100.0f, defaultDryWet, 0.01f, 1.0f));
        parameters.push_back(createFloatParameter(nameDownSample, "DownSample", minSR, maxSR, defaultSR, 1.0f, 0.4f));
        parameters.push_back(createFloatParameter(nameFreqDS, "LFO Frequency DownSample (Hz)", minFreq, maxFreq, defaultFreq, 0.01f, 0.5f));
        parameters.push_back(createFloatParameter(nameAmountDS, "LFO Amount DownSample (Hz)", 0.0f, modSRRange, defaultAmount, 1.0f, 1.0f));
        parameters.push_back(createChoiceParameter(nameWaveformDS, "LFO Waveform DownSample", juce::StringArray{"Sinusoid", "Triangular", "Saw Up", "Saw Down", "Square", "Sample and Hold"}, defaultWaveform));
        parameters.push_back(createFloatParameter(nameDryWetBC, "Dry/Wet BC (%)", 0.0f, 100.0f, defaultDryWet, 0.01f, 1.0f));
        parameters.push_back(createFloatParameter(nameBitCrush, "Bits", minBitDepth, maxBitDepth, defaultBitDepth, 0.001f, 0.5f));
        parameters.push_back(createFloatParameter(nameFreqBC, "LFO Frequency BitCrush (Hz)", minFreq, maxFreq, defaultFreq, 0.01f, 0.5f));
        parameters.push_back(createFloatParameter(nameAmountBC, "LFO Amount BitCrush (bits)", 0.0f, modBitRange, defaultAmount, 0.01f, 1.0f));
        parameters.push_back(createChoiceParameter(nameWaveformBC, "LFO Waveform BitCrush", juce::StringArray{"Sinusoid", "Triangular", "Saw Up", "Saw Down", "Square", "Sample and Hold"}, defaultWaveform));
        parameters.push_back(createFloatParameter(nameGainOut, "Gain OUT", minGain, maxGain, defaultGain, 0.1f, 3.0f));

        return { parameters.begin(), parameters.end() };
    }

    // Add listeners to all parameters
    void addListenerToAllParameters(juce::AudioProcessorValueTreeState& valueTreeState, juce::AudioProcessorValueTreeState::Listener* listener) {
        std::unique_ptr<juce::XmlElement> xml(valueTreeState.copyState().createXml());

        for (auto* element : xml->getChildWithTagNameIterator("PARAM")) {
            const juce::String& id = element->getStringAttribute("id");
            valueTreeState.addParameterListener(id, listener);
        }
    }
}
