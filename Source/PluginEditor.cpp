/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <BinaryData.h>

//==============================================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible (lowFreqSlider);
    lowFreqSlider.setSliderStyle(juce::Slider::Rotary);
    lowFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowFreqSlider.setPopupDisplayEnabled(true, false, this);
    lowFreqSlider.setNumDecimalPlacesToDisplay(2);
    lowFreqSlider.setTextValueSuffix(" Hz");
    lowFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "lowFrequency", lowFreqSlider);

    setSize (1200, 700);
}

SimpleEQAudioProcessorEditor::~SimpleEQAudioProcessorEditor()
{
}

//==============================================================================
void SimpleEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(20,20,20));

    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::AlmendraSCRegular_ttf,BinaryData::AlmendraSCRegular_ttfSize);
    juce::Font titleFont(juce::FontOptions(typeface).withHeight(50.0f));
    juce::Font controlsFont(juce::FontOptions(typeface).withHeight(40.0f));

    auto area = getLocalBounds().reduced(20);

    auto titleArea = area.removeFromTop(60);
    auto responseArea = area.removeFromTop(400);

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


}

void SimpleEQAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);

    auto titleArea = area.removeFromTop(60);
    auto responseArea = area.removeFromTop(400);

    auto lowArea = area.removeFromLeft(area.getWidth() / 3);
    auto lowFreqArea = lowArea.removeFromLeft(lowArea.getWidth() / 3);
    auto midArea = area.removeFromLeft(area.getWidth() / 2);
    auto highArea = area;

    lowFreqSlider.setBounds(lowFreqArea);
}
