/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GraphicEqualizer02AudioProcessorEditor  : 
    public AudioProcessorEditor,
    public Slider::Listener
{
public:
    GraphicEqualizer02AudioProcessorEditor (GraphicEqualizer02AudioProcessor&);
    ~GraphicEqualizer02AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void sliderValueChanged (Slider* slider) override;
    Slider bandGain[MAX_BAND];
    Label bandLabel[MAX_BAND];
    GraphicEqualizer02AudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicEqualizer02AudioProcessorEditor)
};
