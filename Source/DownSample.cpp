
#include "DownSample.h"

DownSample::DownSample()
    : currentSampleRate(44100.0) {}

DownSample::~DownSample() {}

void DownSample::prepareToPlay(double sampleRate, int samplesPerBlock) {
    currentSampleRate = sampleRate;
    aliasingBuffer.setSize(2, samplesPerBlock);
    aliasingBuffer.clear();
    ratio = currentSampleRate;
}

void DownSample::releaseResources() {
    aliasingBuffer.setSize(0, 0);
}

void DownSample::processBlock(AudioBuffer<float>& buffer, AudioBuffer<double>& modulation) {
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();
    auto bufferData = buffer.getArrayOfWritePointers();
    auto aliasingData = aliasingBuffer.getArrayOfWritePointers();
    auto modData = modulation.getArrayOfWritePointers();
    auto numModCh = modulation.getNumChannels();
    int t = 0;
    auto targetSampleRate = 44100;

    for (int smp = 0; smp < numSamples; ++smp) {
        for (int ch = 0; ch < numChannels; ++ch) {
            targetSampleRate = jmin(modData[jmin(ch, numModCh - 1)][smp], 44100.0);
            ratio = currentSampleRate / targetSampleRate;
            
            aliasingData[ch][smp] = (t == 0) ? bufferData[ch][smp] : aliasingData[ch][smp - (smp > 0)];
            bufferData[ch][smp] += aliasingData[ch][smp];
        }

        t++;
        if (t >= ratio) t = 0;
    }
}


