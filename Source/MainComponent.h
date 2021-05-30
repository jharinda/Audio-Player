#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;


private:
    //==============================================================================
    // Your private member variables go here...
    juce::AudioDeviceManager otherDeviceManager;
    std::unique_ptr <juce::AudioDeviceSelectorComponent> audioSettings;

    enum TransportState
    {
        Stopped,
        Playing,
        Stopping,
        Starting
    };
    
    TransportState state;
    
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    juce::AudioFormatManager formatManager;

    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transport;

    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    void transportStateChanged(TransportState state);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
