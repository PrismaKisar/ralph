#pragma once
#include <JuceHeader.h>
#include <random>
#include <ctime>

constexpr int SINUSOID = 0;
constexpr int TRIANGULAR = 1;
constexpr int SAW_UP = 2;
constexpr int SAW_DOWN = 3;
constexpr int SQUARE = 4;
constexpr int SAMPLE_AND_HOLD = 5;

class Oscillator {
public:
    Oscillator(double defaultFrequency = 1.0, int defaultWaveform = SINUSOID);
    ~Oscillator() = default;

    void prepareToPlay(double sampleRate);
    void setFrequency(double newValue);
    void setWaveform(int newValue);
    void getNextAudioBlock(AudioBuffer<double>& buffer, int numSamples);
    float getNextAudioSample();
        
private:
    int waveform;
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> frequency;

    double currentPhase;
    double samplePeriod;
    float prevValue;
    bool newCycle;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis{-1.0, 1.0};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator)
};
