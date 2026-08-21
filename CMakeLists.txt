cmake_minimum_required(VERSION 3.25)

project(SimpleEQ)

include(FetchContent)

FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.12
)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SimpleEQ
        PRODUCT_NAME "SimpleEQ"
        COMPANY_NAME "Zajamelia"
        VERSION 0.0.1

        PLUGIN_MANUFACTURER_CODE Zaja
        PLUGIN_CODE SmEQ

        FORMATS
        Standalone
        VST3

        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
)
juce_generate_juce_header(SimpleEQ)

target_sources(SimpleEQ
        PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
)

target_link_libraries(SimpleEQ
        PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
)