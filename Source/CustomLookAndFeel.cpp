#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel() {
    littleKnob = juce::ImageFileFormat::loadFrom(BinaryData::littleKnob_png, BinaryData::littleKnob_pngSize);
    holeImage = juce::ImageFileFormat::loadFrom(BinaryData::hole_png, BinaryData::hole_pngSize);
    knobWithoutPointer = juce::ImageFileFormat::loadFrom(BinaryData::knobWithoutPointer_png, BinaryData::knobWithoutPointer_pngSize);
    pointer = juce::ImageFileFormat::loadFrom(BinaryData::pointer_png, BinaryData::pointer_pngSize);
}

void CustomLookAndFeel::drawRotaryTicks(Graphics& g, float centreX, float centreY, float radius, float rotaryStartAngle, float rotaryEndAngle, int numTicks) {
    const float tickLength = 3.0f;
    const float tickThickness = 2.0f;
    const Colour tickColor = Colours::grey;

    Path originalShape, tempShape;
    originalShape.addRectangle(tickThickness * -0.5f, -radius, tickThickness, tickLength);
    
    g.setColour(tickColor);
    for (int i = 0; i < numTicks; ++i) {
        tempShape = originalShape;
        float tickAngle = rotaryStartAngle + (i / (float)(numTicks - 1)) * (rotaryEndAngle - rotaryStartAngle);
        tempShape.applyTransform(AffineTransform::rotation(tickAngle).translated(centreX, centreY));
        g.fillPath(tempShape);
    }
}

void CustomLookAndFeel::drawRotaryKnob(Graphics& g, int x, int y, int width, int height) {
    const float scaleFactor = 0.8f;
    const int reducedWidth = static_cast<int>(width * scaleFactor);
    const int reducedHeight = static_cast<int>(height * scaleFactor);
    const int offsetX = x + (width - reducedWidth) / 2;
    const int offsetY = y + (height - reducedHeight) / 2;
    
    g.drawImageWithin(knobWithoutPointer, offsetX, offsetY, reducedWidth, reducedHeight, juce::RectanglePlacement::centred);
}

void CustomLookAndFeel::drawRotaryPointer(Graphics& g, float rotation, int x, int y, int width, int height) {
    const float scaleFactor = 0.8f;
    const int reducedWidth = static_cast<int>(width * scaleFactor);
    const int reducedHeight = static_cast<int>(height * scaleFactor);
    const int offsetX = x + (width - reducedWidth) / 2;
    const int offsetY = y + (height - reducedHeight) / 2;
    const float pointerScaleRelativeToKnob = 0.28f;
    const int pointerWidth = static_cast<int>(reducedWidth * pointerScaleRelativeToKnob);
    const int pointerHeight = static_cast<int>(reducedHeight * pointerScaleRelativeToKnob);
    const float reducedRadius = reducedWidth * 0.35f;

    g.saveState();
    g.addTransform(juce::AffineTransform::translation(offsetX + reducedWidth / 2, offsetY + reducedHeight / 2));
    g.addTransform(juce::AffineTransform::rotation(rotation));
    g.drawImageWithin(pointer, -pointerWidth / 2, -reducedRadius, pointerWidth, pointerHeight, juce::RectanglePlacement::centred);
    g.restoreState();
}

void CustomLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) {
    float radius = jmin(width, height) * 0.5f;
    float centreX = x + width * 0.5f;
    float centreY = y + height * 0.5f;

    drawRotaryTicks(g, centreX, centreY, radius, rotaryStartAngle, rotaryEndAngle, numTicks);
    drawRotaryKnob(g, x, y, width, height);
    
    const double rotation = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    drawRotaryPointer(g, rotation, x, y, width, height);

    drawTimedSliderOverlay(g, slider, width, height);
}

void CustomLookAndFeel::drawLinearTicks(Graphics& g, int x, int width) {
    g.setColour(Colours::grey);
    float tickHeight = 3.0f;
    float extendedTickHeight = tickHeight * 2;
    float tickWidth = (width - 5) / 20.0f;

    for (int i = 0; i < 20; ++i) {
        float tickX = (x + 5) + i * tickWidth;
        float currentTickHeight = (i == 0 || i == 19) ? extendedTickHeight : tickHeight;
        g.drawLine(tickX, 0, tickX, currentTickHeight, 2.0f);
    }
   
    // Delimitare il contesto grafico in rosso
    g.setColour(Colours::red);
    //g.drawRect(static_cast<float>(x), 0.0f, static_cast<float>(width), extendedTickHeight, 2.0f);
}

void CustomLookAndFeel::drawLinearKnob(Graphics& g, float knobX) {
    g.setOpacity(1);
    g.drawImageWithin(littleKnob, static_cast<int>(knobX), 5, 20, 20, RectanglePlacement::stretchToFit);
}

void CustomLookAndFeel::drawLinearSlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, Slider::SliderStyle sliderStyle, Slider &slider) {
    g.setOpacity(0.5);
    g.drawImageWithin(holeImage, x + 2, y + 11, width - 4, 7, juce::RectanglePlacement::stretchToFit);

    maxSliderPos = 188;
    float proportion = (sliderPos - minSliderPos) / (maxSliderPos - minSliderPos);
    float knobX = x + proportion * (width - 20); // Assicura che il knob copra l'intera larghezza dell'immagine

    // Limita knobX entro i limiti del contesto grafico
    knobX = jlimit(static_cast<float>(x), static_cast<float>(x + width - 20), knobX);

    drawLinearTicks(g, x, width);
    drawLinearKnob(g, knobX);

    drawTimedSliderOverlay(g, slider, width, height);

    // Rettangolo di debug
}



void CustomLookAndFeel::drawTimedSliderOverlay(Graphics& g, Slider& slider, int width, int height) {
    if (auto* timedSlider = dynamic_cast<TimedSlider*>(&slider)) {
        
        timedSlider->setValueWithTimeCheck(slider.getValue());
        if (!timedSlider->isDrawable()) return;
        
        String sliderValue = String(timedSlider->getValue());

        int rectWidth = jmax(width / 2, 30);
        int rectHeight = jmax(height / 4, 20);
        int rectX = (width - rectWidth) / 2;
        int rectY = (height - rectHeight);

        Colour rectColor = Colour(0x70555555);
        Colour textColor = Colours::white;

        g.setColour(rectColor);
        g.fillRoundedRectangle(rectX, rectY, rectWidth, rectHeight, 5.0f);

        float fontSize = jmin(10.0f, (float)height * 0.5f);
        g.setFont(Font(fontSize));

        g.setOpacity(1);
        g.setColour(textColor);
        g.drawFittedText(sliderValue, rectX, rectY, rectWidth, rectHeight, Justification::centred, 1);
    }
}



