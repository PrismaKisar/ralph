#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "Meter.h"
#include "TimedSlider.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class RalphAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    RalphAudioProcessorEditor(RalphAudioProcessor&, AudioProcessorValueTreeState&);
    ~RalphAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RalphAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& parameters;

    CustomLookAndFeel lookAndFeel, lookAndFeelLessTick;

    std::unique_ptr<Meter> meterIN, meterOUT;

    Image backgroundTexture, glassTexture, screwImage;
    Image ralphWrite, bitCrushWrite, downSampleWrite;
    Image bitsWrite, hertzWrite, frequencyWrite, amountWrite, dryWetWrite, waveformWrite;
    Image zeroImage, hundredImage, pointOOneImage, sixtyImage, fourImage, tenKImage, twoImage, twentyFourImage, fiveHundredImage, fourFourKImage;
    Image triImage, sinImage, sawUpImage, sawDownImage, quadImage, shImage;

    TimedSlider gainINSlider, gainOUTSlider;
    TimedSlider BitCrushSlider, AmountBCSlider, FreqBCSlider, DryWetBCSlider;
    TimedSlider DownSampleSlider, AmountDSSlider, FreqDSSlider, DryWetDSSlider;
    Slider WaveformBCSlider, WaveformDSSlider;

    std::unique_ptr<SliderAttachment> gainINAttachment, gainOUTAttachment;
    std::unique_ptr<SliderAttachment> BitCrushAttachment, AmountBCAttachment, FreqBCAttachment, DryWetBCAttachment, WaveformBCAttachment;
    std::unique_ptr<SliderAttachment> DownSampleAttachment, DryWetDSAttachment, FreqDSAttachment, AmountDSAttachment, WaveformDSAttachment;

    void setupSlider(Slider& slider, Slider::SliderStyle style, int x, int y, int w, int h, CustomLookAndFeel& lookAndFeel);
    
    void drawBackground(Graphics& g);
    void drawMacroSections(Graphics& g);
    void drawMeters(Graphics& g);
    void drawTexts(Graphics& g);
    void drawTextures(Graphics& g);
    void drawScrews(Graphics& g);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RalphAudioProcessorEditor)
};
