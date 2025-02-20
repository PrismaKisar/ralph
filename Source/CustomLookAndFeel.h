#pragma once

#include <JuceHeader.h>
#include "TimedSlider.h"

class CustomLookAndFeel : public LookAndFeel_V4 {
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel() {}

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, Slider::SliderStyle sliderStyle, Slider& slider) override;
    void setNumTicks(int numTicks) { this->numTicks = numTicks; }

private:
    Image littleKnob;
    Image holeImage;
    Image knobWithoutPointer;
    Image pointer;

    int numTicks = 15;

    void drawRotaryTicks(Graphics& g, float centreX, float centreY, float radius, float rotaryStartAngle, float rotaryEndAngle, int numTicks);
    void drawRotaryKnob(Graphics& g, int x, int y, int width, int height);
    void drawRotaryPointer(Graphics& g, float rotation, int x, int y, int width, int height);
    void drawLinearTicks(Graphics& g, int x, int width);
    void drawLinearKnob(Graphics& g, float knobX);
    void drawTimedSliderOverlay(Graphics& g, Slider& slider, int width, int height);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
