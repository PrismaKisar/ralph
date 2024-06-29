#include "BitCrush.h"

void BitCrush::processBlock(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<double>& modulation) {
    const auto numSamples = buffer.getNumSamples();
    const auto numCh = buffer.getNumChannels();

    auto bufferData = buffer.getArrayOfWritePointers();
    auto modData = modulation.getArrayOfWritePointers();
    const auto numModCh = modulation.getNumChannels();

    for (int smp = 0; smp < numSamples; ++smp)
        for (int ch = 0; ch < numCh; ++ch)
            bufferData[ch][smp] = crush(bufferData[ch][smp], jmin(modData[jmin(ch, numModCh - 1)][smp], 24.0)
 );
}

float BitCrush::crush(float value, double bits) {
    const double QL = 2.0 / (pow(2, bits) - 1);
    return QL * static_cast<int>(value / QL);
}
