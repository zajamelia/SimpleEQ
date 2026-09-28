/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SimpleEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor&);
    ~SimpleEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void setupSlider(juce::Slider& slider);
    SimpleEQAudioProcessor& audioProcessor;

    juce::Slider lowFreqSlider;
    juce::Label lowFreqLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowFreqAttachment;

    juce::Slider lowGainSlider;
    juce::Label lowGainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowGainAttachment;

    juce::Slider lowQSlider;
    juce::Label lowQLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowQAttachment;

    juce::Slider midFreqSlider;
    juce::Label midFreqLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midFreqAttachment;

    juce::Slider midGainSlider;
    juce::Label midGainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midGainAttachment;

    juce::Slider midQSlider;
    juce::Label midQLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midQAttachment;

    juce::Slider highFreqSlider;
    juce::Label highFreqLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highFreqAttachment;

    juce::Slider highGainSlider;
    juce::Label highGainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highGainAttachment;

    juce::Slider highQSlider;
    juce::Label highQLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highQAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEQAudioProcessorEditor)
};
