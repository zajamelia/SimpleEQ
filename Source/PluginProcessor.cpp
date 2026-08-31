/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"


#include "PluginEditor.h"

//==============================================================================
SimpleEQAudioProcessor::SimpleEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
,
        parameters(*this,nullptr,"Parameters", createParameterLayout())
{
    //Listens for changes in low band parameters
    parameters.addParameterListener("lowFrequency", this);
    parameters.addParameterListener("lowGain", this);
    parameters.addParameterListener("lowQ", this);
    //Listens for changes in mid band parameters
    parameters.addParameterListener("midFrequency", this);
    parameters.addParameterListener("midGain", this);
    parameters.addParameterListener("midQ", this);
    //Listens for changes in high band parameters
    parameters.addParameterListener("highFrequency", this);
    parameters.addParameterListener("highGain", this);
    parameters.addParameterListener("highQ", this);
}

//Creates all plugin parameters
juce::AudioProcessorValueTreeState::ParameterLayout
SimpleEQAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //Low frequency parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>("lowFrequency",
        "Low Frequency",
        juce::NormalisableRange<float>(20.0f,500.0f,1.0f),
        100.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
    "lowGain",
    "Low Gain",
    juce::NormalisableRange<float>(-12.0f, 12.0f,0.1f),
    0.0f
));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
    "lowQ",
    "Low Q",
    juce::NormalisableRange<float>(0.5f, 10.0f,0.1f),
    1.0f
));
    //Mid-frequency parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>("midFrequency",
        "Mid Frequency",
        juce::NormalisableRange<float>(500.f,5000.0f,1.0f),
        1000.0f
));
    layout.add(std::make_unique<juce::AudioParameterFloat>("midGain",
        "Mid Gain",
        juce::NormalisableRange<float>(-12.0f, 12.0f,0.1f),
        0.f
));
    layout.add(std::make_unique<juce::AudioParameterFloat>("midQ",
        "Mid Q",
        juce::NormalisableRange<float>(0.5f, 10.0f,0.1f),
        1.0f));
    // High frequency parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>("highFrequency",
           "High frequency",
           juce::NormalisableRange<float>(5000.f,20000.0f,1.0f),
           10000.0f
));
    layout.add(std::make_unique<juce::AudioParameterFloat>("highGain",
        "High gain",
        juce::NormalisableRange<float>(-12.0f, 12.0f,0.1f),
        0.f
));
    layout.add(std::make_unique<juce::AudioParameterFloat>("highQ",
        "High Q",
        juce::NormalisableRange<float>(0.5f, 10.0f,0.1f),
        1.0f));
    return layout;
}

// Updates filter using its smoothed parameters
void SimpleEQAudioProcessor::updateFilter(
 juce::dsp::ProcessorDuplicator<
 juce::dsp::IIR::Filter<float>,
 juce::dsp::IIR::Coefficients<float>>& filter,
 SmoothedFilterParameters& params) {
    // Read parameter values
    const auto frequency = params.frequency.getCurrentValue();
    const auto gain =  params.gain.getCurrentValue();
    const auto q = params.q.getCurrentValue();

    // calculate coefficients
    auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        getSampleRate(),
        frequency,
        q,
        juce::Decibels::decibelsToGain(gain));

    // Give coefficients to chosen filter
    *filter.state = *coefficients;

}

    void SimpleEQAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) {

    // Gives changed parameter's smoother its new target value
    if (parameterID == "lowFrequency") lowParams.frequency.setTargetValue(newValue);
    else if (parameterID == "lowGain") lowParams.gain.setTargetValue(newValue);
    else if (parameterID == "lowQ") lowParams.q.setTargetValue(newValue);
    else if (parameterID == "midFrequency") midParams.frequency.setTargetValue(newValue);
    else if (parameterID == "midGain") midParams.gain.setTargetValue(newValue);
    else if (parameterID == "midQ") midParams.q.setTargetValue(newValue);
    else if (parameterID == "highFrequency") highParams.frequency.setTargetValue(newValue);
    else if (parameterID == "highGain") highParams.gain.setTargetValue(newValue);
    else if (parameterID == "highQ") highParams.q.setTargetValue(newValue);

    if (parameterID.startsWith("low")) lowFilterNeedsUpdate = true;
    else if (parameterID.startsWith("mid")) midFilterNeedsUpdate = true;
    else if (parameterID.startsWith("high")) highFilterNeedsUpdate = true;
}
    SimpleEQAudioProcessor::~SimpleEQAudioProcessor()
    {
    }

    //==============================================================================
    const juce::String SimpleEQAudioProcessor::getName() const
    {
        return JucePlugin_Name;
    }

    bool SimpleEQAudioProcessor::acceptsMidi() const
    {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }

    bool SimpleEQAudioProcessor::producesMidi() const
    {
#if JucePlugin_ProducesMidiOutput
        return true;
#else
        return false;
#endif
    }

    bool SimpleEQAudioProcessor::isMidiEffect() const
    {
#if JucePlugin_IsMidiEffect
        return true;
#else
        return false;
#endif
    }

    double SimpleEQAudioProcessor::getTailLengthSeconds() const
    {
        return 0.0;
    }

    int SimpleEQAudioProcessor::getNumPrograms()
    {
        return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
    }

    int SimpleEQAudioProcessor::getCurrentProgram()
    {
        return 0;
    }

    void SimpleEQAudioProcessor::setCurrentProgram (int index)
    {
    }

    const juce::String SimpleEQAudioProcessor::getProgramName (int index)
    {
        return {};
    }

    void SimpleEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
    {
    }

    //==============================================================================
    void SimpleEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
    {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
        spec.numChannels = getTotalNumOutputChannels();

        lowFilter.prepare(spec);
        midFilter.prepare(spec);
        highFilter.prepare(spec);

        // Sets smoothing time for all filters to 20ms
        lowParams.frequency.reset(sampleRate,0.02);
        lowParams.gain.reset(sampleRate,0.02);
        lowParams.q.reset(sampleRate,0.02);

        midParams.frequency.reset(sampleRate,0.02);
        midParams.gain.reset(sampleRate,0.02);
        midParams.q.reset(sampleRate,0.02);

        highParams.frequency.reset(sampleRate,0.02);
        highParams.gain.reset(sampleRate,0.02);
        highParams.q.reset(sampleRate,0.02);

    // Set smoothers to their starting parameter values
    lowParams.frequency.setCurrentAndTargetValue(parameters.getRawParameterValue("lowFrequency")->load());
    lowParams.gain.setCurrentAndTargetValue(parameters.getRawParameterValue("lowGain")->load());
    lowParams.q.setCurrentAndTargetValue(parameters.getRawParameterValue("lowQ")->load());

    midParams.frequency.setCurrentAndTargetValue(parameters.getRawParameterValue("midFrequency")->load());
    midParams.gain.setCurrentAndTargetValue(parameters.getRawParameterValue("midGain")->load());
    midParams.q.setCurrentAndTargetValue(parameters.getRawParameterValue("midQ")->load());

    highParams.frequency.setCurrentAndTargetValue(parameters.getRawParameterValue("highFrequency")->load());
    highParams.gain.setCurrentAndTargetValue(parameters.getRawParameterValue("highGain")->load());
    highParams.q.setCurrentAndTargetValue(parameters.getRawParameterValue("highQ")->load());



    // Set filter coefficients
    updateFilter(lowFilter, lowParams);
    updateFilter(midFilter, midParams);
    updateFilter(highFilter, highParams);
    }

    void SimpleEQAudioProcessor::releaseResources() {
        // When playback stops, you can use this as an opportunity to free up any
        // spare memory, etc.
    }

#ifndef JucePlugin_PreferredChannelConfigurations
    bool SimpleEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
    {
#if JucePlugin_IsMidiEffect
        juce::ignoreUnused (layouts);
        return true;
#else
        // This is the place where you check if the layout is supported.
        // In this template code we only support mono or stereo.
        // Some plugin hosts, such as certain GarageBand versions, will only
        // load plugins that support stereo bus layouts.
        if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
         && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
            return false;

        // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
        if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
            return false;
#endif

        return true;
#endif
    }
#endif

    void SimpleEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
    {
        juce::ScopedNoDenormals noDenormals;
        const auto numSamples = buffer.getNumSamples();  //Gets number of samples in the current audio block
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();

        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());

        juce::dsp::AudioBlock<float> block (buffer); // Give DSP system access to buffer
        juce::dsp::ProcessContextReplacing<float> context (block); // // Replace the original audio with the processed audio
    const bool anyFilterIsSmoothing = lowParams.frequency.isSmoothing() || lowParams.gain.isSmoothing() || lowParams.q.isSmoothing() ||
        midParams.frequency.isSmoothing() ||midParams.gain.isSmoothing() ||midParams.q.isSmoothing() ||
        highParams.frequency.isSmoothing() ||highParams.gain.isSmoothing() || highParams.q.isSmoothing();

    // If nothing is changing process the block normally
    if (!anyFilterIsSmoothing) {
        lowFilter.process(context);
        midFilter.process(context);
        highFilter.process(context);
        return;
    }

    //If something is changing process sample by sample
      for (int sample = 0; sample < numSamples; ++sample) {
          const bool lowIsSmoothing =
    lowParams.frequency.isSmoothing() ||
    lowParams.gain.isSmoothing() ||
    lowParams.q.isSmoothing();

          const bool midIsSmoothing =
              midParams.frequency.isSmoothing() ||
              midParams.gain.isSmoothing() ||
              midParams.q.isSmoothing();

          const bool highIsSmoothing =
              highParams.frequency.isSmoothing() ||
              highParams.gain.isSmoothing() ||
              highParams.q.isSmoothing();

          lowParams.frequency.getNextValue();
          lowParams.gain.getNextValue();
          lowParams.q.getNextValue();

          midParams.frequency.getNextValue();
          midParams.gain.getNextValue();
          midParams.q.getNextValue();

          highParams.frequency.getNextValue();
          highParams.gain.getNextValue();
          highParams.q.getNextValue();

          if (lowIsSmoothing)
          {
              updateFilter(lowFilter, lowParams);
          }

          if (midIsSmoothing)
          {
              updateFilter(midFilter, midParams);
          }

          if (highIsSmoothing)
          {
              updateFilter(highFilter, highParams);
          }

          auto singleSampleBlock = block.getSubBlock(sample,1);
          juce::dsp::ProcessContextReplacing<float> singleSamplecontext (singleSampleBlock);

          lowFilter.process(singleSamplecontext);
          midFilter.process(singleSamplecontext);
          highFilter.process(singleSamplecontext);
      }
    }

    //==============================================================================
    bool SimpleEQAudioProcessor::hasEditor() const
    {
        return true; // (change this to false if you choose to not supply an editor)
    }

    juce::AudioProcessorEditor* SimpleEQAudioProcessor::createEditor()
    {
        return new juce::GenericAudioProcessorEditor (*this);
    }

    //==============================================================================
    void SimpleEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
    {
        auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
    }

    void SimpleEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
    {
        std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr) {
        if (xmlState->hasTagName(parameters.state.getType())) {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
    }

    //==============================================================================
    // This creates new instances of the plugin..
    juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
    {
        return new SimpleEQAudioProcessor();
    }
