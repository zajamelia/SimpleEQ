/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <BinaryData.h>
#include <array>
#include <cmath>

//==============================================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::AlmendraSCRegular_ttf, BinaryData::AlmendraSCRegular_ttfSize);
    juce::Font labelFont(juce::FontOptions(typeface).withHeight(15.0f));

    setupSlider(lowFreqSlider);
    lowFreqLabel.setText("Frequency", juce::dontSendNotification);
    addAndMakeVisible(lowFreqLabel);
    lowFreqLabel.setFont(labelFont);
    lowFreqLabel.setJustificationType(juce::Justification::centred);
    lowFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "lowFrequency", lowFreqSlider);

    setupSlider(lowGainSlider);
    lowGainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible(lowGainLabel);
    lowGainLabel.setFont(labelFont);
    lowGainLabel.setJustificationType(juce::Justification::centred);
    lowGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "lowGain", lowGainSlider);


    setupSlider(lowQSlider);
    lowQLabel.setText("Q", juce::dontSendNotification);
    addAndMakeVisible(lowQLabel);
    lowQLabel.setFont(labelFont);
    lowQLabel.setJustificationType(juce::Justification::centred);
    lowQAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "lowQ", lowQSlider);

    setupSlider(midFreqSlider);
    midFreqLabel.setText("Frequency", juce::dontSendNotification);
    addAndMakeVisible(midFreqLabel);
    midFreqLabel.setFont(labelFont);
    midFreqLabel.setJustificationType(juce::Justification::centred);
    midFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "midFrequency", midFreqSlider);

    setupSlider(midGainSlider);
    midGainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible(midGainLabel);
    midGainLabel.setFont(labelFont);
    midGainLabel.setJustificationType(juce::Justification::centred);
    midGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "midGain", midGainSlider);

    setupSlider(midQSlider);
    midQLabel.setText("Q", juce::dontSendNotification);
    addAndMakeVisible(midQLabel);
    midQLabel.setFont(labelFont);
    midQLabel.setJustificationType(juce::Justification::centred);
    midQAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "midQ", midQSlider);

    setupSlider(highFreqSlider);
    highFreqLabel.setText("Frequency", juce::dontSendNotification);
    addAndMakeVisible(highFreqLabel);
    highFreqLabel.setFont(labelFont);
    highFreqLabel.setJustificationType(juce::Justification::centred);
    highFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "highFrequency", highFreqSlider);

    setupSlider(highGainSlider);
    highGainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible(highGainLabel);
    highGainLabel.setFont(labelFont);
    highGainLabel.setJustificationType(juce::Justification::centred);
    highGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "highGain", highGainSlider);

    setupSlider(highQSlider);
    highQLabel.setText("Q", juce::dontSendNotification);
    addAndMakeVisible(highQLabel);
    highQLabel.setFont(labelFont);
    highQLabel.setJustificationType(juce::Justification::centred);
    highQAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "highQ", highQSlider);
    setSize (800, 500);


}

SimpleEQAudioProcessorEditor::~SimpleEQAudioProcessorEditor()
{
}

//==============================================================================
void SimpleEQAudioProcessorEditor::setupSlider(juce::Slider& slider)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setPopupDisplayEnabled(true, false, this);
    slider.setNumDecimalPlacesToDisplay(2);

    slider.onValueChange = [this] { repaint(); };
}
void SimpleEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(20,20,20));

    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::AlmendraSCRegular_ttf,BinaryData::AlmendraSCRegular_ttfSize);
    juce::Font titleFont(juce::FontOptions(typeface).withHeight(50.0f));
    juce::Font controlsFont(juce::FontOptions(typeface).withHeight(25.0f));

    auto area = getLocalBounds().reduced(20);

    auto titleArea = area.removeFromTop(60);
    auto responseArea = area.removeFromTop(200);

    auto lowArea = area.removeFromLeft(area.getWidth()/3);
    auto midArea = area.removeFromLeft(area.getWidth()/2);
    auto highArea = area;

    g.setFont(titleFont);
    g.setColour(juce::Colours::lightgrey);
    g.drawText("SimpleEQ", titleArea, juce::Justification::centred);
    g.setFont(controlsFont);
    g.drawText("Low",lowArea, juce::Justification::centredTop);
    g.drawText("Mid",midArea, juce::Justification::centredTop);
    g.drawText("High",highArea, juce::Justification::centredTop);
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(titleArea);
    g.drawRect(responseArea);
    g.drawRect(lowArea);
    g.drawRect(midArea);
    g.drawRect(highArea);

    auto graphArea = responseArea.reduced(40, 15);

    g.setColour(juce::Colour(35, 35, 35));
    g.fillRect(graphArea);

    g.setColour(juce::Colours::darkgrey);
    g.drawRect(graphArea);

    // 0 dB centre line
    auto zeroY = graphArea.getCentreY();

    g.setColour(juce::Colours::grey);
    g.drawHorizontalLine(zeroY,
                         (float) graphArea.getX(),
                         (float) graphArea.getRight());

    // +12 dB / 0 dB / -12 dB labels
    g.setColour(juce::Colours::lightgrey);
    g.setFont(12.0f);

    g.drawText("+12 dB",
               responseArea.getX(),
               graphArea.getY() - 7,
               38, 15,
               juce::Justification::centredRight);

    g.drawText("0 dB",
               responseArea.getX(),
               zeroY - 7,
               38, 15,
               juce::Justification::centredRight);

    g.drawText("-12 dB",
               responseArea.getX(),
               graphArea.getBottom() - 7,
               38, 15,
               juce::Justification::centredRight);

    const std::array<float, 4> frequencies =
    {
        100.0f,
        1000.0f,
        10000.0f,
        20000.0f
    };

    for (auto frequency : frequencies)
    {
        auto normX = std::log10(frequency / 20.0f) / std::log10(20000.0f / 20.0f);
        auto x = graphArea.getX() + normX * graphArea.getWidth();

        // Vertical grid line
        g.setColour(juce::Colour(55, 55, 55));
        g.drawVerticalLine((int) x,(float) graphArea.getY(),(float) graphArea.getBottom());

        // Frequency label
        juce::String label;
        if (frequency >= 1000.0f)
            label = juce::String(frequency / 1000.0f, 0) + "k";
        else
            label = juce::String((int) frequency);

        g.setColour(juce::Colours::lightgrey);
        g.setFont(12.0f);

        g.drawText(label,
                   (int) x - 25,
                   graphArea.getBottom() - 20,
                   50,
                   20,
                   juce::Justification::centred);
    }
    // Frequency response code
    juce::Path responseCurve;

    auto sampleRate = audioProcessor.getSampleRate();
    if (sampleRate <= 0)
        sampleRate = 44100.0;
    //Get slider values
    auto lowFreq = lowFreqSlider.getValue();
    auto lowGain = lowGainSlider.getValue();
    auto lowQ = lowQSlider.getValue();

    auto midFreq = midFreqSlider.getValue();
    auto midGain = midGainSlider.getValue();
    auto midQ = midQSlider.getValue();

    auto highFreq = highFreqSlider.getValue();
    auto highGain = highGainSlider.getValue();
    auto highQ = highQSlider.getValue();

    auto lowCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate,lowFreq,lowQ,juce::Decibels::decibelsToGain(lowGain));

    auto midCoefficients =juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            sampleRate,midFreq,midQ,juce::Decibels::decibelsToGain(midGain));

    auto highCoefficients =juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            sampleRate, highFreq,highQ,juce::Decibels::decibelsToGain(highGain));

    for (int x = graphArea.getX(); x < graphArea.getRight(); ++x)
    {
        // Position across the graph from 0 to 1
        auto normX = (float) (x - graphArea.getX()) / (float) graphArea.getWidth();

        // Convert position to a frequency between 20 Hz and 20 kHz
        auto frequency = 20.0 * std::pow(20000.0 / 20.0, normX);

        // Get response of each EQ band at this frequency
        auto lowMagnitude = lowCoefficients->getMagnitudeForFrequency(frequency, sampleRate);
        auto midMagnitude = midCoefficients->getMagnitudeForFrequency(frequency, sampleRate);
        auto highMagnitude = highCoefficients->getMagnitudeForFrequency(frequency, sampleRate);

        // Combine the three filters and convert to dB
        auto magnitude = lowMagnitude * midMagnitude * highMagnitude;
        auto magnitudeDB = juce::Decibels::gainToDecibels(magnitude);

        // Convert dB value to vertical position
        auto y = juce::jmap((float) magnitudeDB, -12.0f, 12.0f,
                            (float) graphArea.getBottom(), (float) graphArea.getY());

        // Add point to the response curve
        if (x == graphArea.getX())
            responseCurve.startNewSubPath((float) x, y);
        else
            responseCurve.lineTo((float) x, y);
    }

    // Draw the completed response curve
    g.setColour(juce::Colours::darkred);
    g.reduceClipRegion(graphArea);
    g.strokePath(responseCurve, juce::PathStrokeType(2.0f));
}

void SimpleEQAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);

    // Title and graph
    area.removeFromTop(60);
    area.removeFromTop(200);

    // Split vertically
    auto lowArea = area.removeFromLeft(area.getWidth() / 3);
    auto midArea = area.removeFromLeft(area.getWidth() / 2);
    auto highArea = area;

    // room for Low / Mid / High headings
    lowArea.removeFromTop(35);
    midArea.removeFromTop(35);
    highArea.removeFromTop(35);

    // LOW controls
    auto lowFreqArea = lowArea.removeFromTop(50);
    lowFreqLabel.setBounds(lowFreqArea.removeFromTop(18));
    lowFreqSlider.setBounds(lowFreqArea);

    auto lowGainArea = lowArea.removeFromTop(50);
    lowGainLabel.setBounds(lowGainArea.removeFromTop(18));
    lowGainSlider.setBounds(lowGainArea);

    auto lowQArea = lowArea.removeFromTop(50);
    lowQLabel.setBounds(lowQArea.removeFromTop(18));
    lowQSlider.setBounds(lowQArea);

    // MID controls
    auto midFreqArea = midArea.removeFromTop(50);
    midFreqLabel.setBounds(midFreqArea.removeFromTop(18));
    midFreqSlider.setBounds(midFreqArea);

    auto midGainArea = midArea.removeFromTop(50);
    midGainLabel.setBounds(midGainArea.removeFromTop(18));
    midGainSlider.setBounds(midGainArea);

    auto midQArea = midArea.removeFromTop(50);
    midQLabel.setBounds(midQArea.removeFromTop(18));
    midQSlider.setBounds(midQArea);

    // HIGH controls
    auto highFreqArea = highArea.removeFromTop(50);
    highFreqLabel.setBounds(highFreqArea.removeFromTop(18));
    highFreqSlider.setBounds(highFreqArea);

    auto highGainArea = highArea.removeFromTop(50);
    highGainLabel.setBounds(highGainArea.removeFromTop(18));
    highGainSlider.setBounds(highGainArea);

    auto highQArea = highArea.removeFromTop(50);
    highQLabel.setBounds(highQArea.removeFromTop(18));
    highQSlider.setBounds(highQArea);


}
