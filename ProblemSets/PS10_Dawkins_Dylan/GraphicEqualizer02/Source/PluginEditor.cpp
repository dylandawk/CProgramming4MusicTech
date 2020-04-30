/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define SLIDER_W 30
#define SLIDER_H 200
#define SLIDER_SPACING 60
#define H_MARGIN 30
#define V_MARGIN 50
#define PANEL_W (SLIDER_SPACING * MAX_BAND + 2 * H_MARGIN)
#define PANEL_H (SLIDER_H + 2 * V_MARGIN)
#define VERSION "v1.8"

//==============================================================================
GraphicEqualizer02AudioProcessorEditor::GraphicEqualizer02AudioProcessorEditor (GraphicEqualizer02AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setSize (PANEL_W, PANEL_H);
    
    char s[8];
    for (int i = 0; i < MAX_BAND; i++){
        
        
        // Set Slider Attributes
        bandGain[i].setSliderStyle(Slider::LinearVertical);
        bandGain[i].setRange(MIN_GAIN_DB, 0, 3);
        bandGain[i].setTextBoxStyle(Slider::TextBoxBelow, true, SLIDER_SPACING, 20);
        bandGain[i].setValue(INIT_GAIN_DB);
        
         // Add Slider to GUI
        addAndMakeVisible(&bandGain[i]);
        
        // Set Slider Text Label
        bandLabel[i].setText(processor.equalizer.get_band_cf_str(i, s), sendNotification);
        bandLabel[i].attachToComponent(&bandGain[i], false);
        addAndMakeVisible(bandLabel[i]);
        
        // Add a Listener to Slider
        bandGain[i].addListener(this);
    }
}

GraphicEqualizer02AudioProcessorEditor::~GraphicEqualizer02AudioProcessorEditor()
{
}

//==============================================================================
void GraphicEqualizer02AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setColour (Colours::white);
        g.setFont (15.0f);
        
    g.drawFittedText("Band Center Frequencies", getLocalBounds(), Justification::centredTop, 1); 
}

void GraphicEqualizer02AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    for (int i = 0; i < MAX_BAND; i++){
        bandGain[i].setBounds(H_MARGIN + i * SLIDER_SPACING, PANEL_H / 2 - V_MARGIN, SLIDER_W, SLIDER_H);
    }
}

void GraphicEqualizer02AudioProcessorEditor::sliderValueChanged (Slider*
slider)
{
    int i = (int)(slider - &bandGain[0]);
    processor.equalizer.set_gain_dB(i, slider->getValue());    
}
