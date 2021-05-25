#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :openButton("Open")
{
 
    setSize (200, 100);


    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
   
        setAudioChannels (0, 2);
    }

    openButton.onClick = [this] { openButtonClicked(); };
}

MainComponent::~MainComponent()
{
   
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
}

void MainComponent::openButtonClicked()
{
    DBG("clicked");
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
   
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    
}

void MainComponent::resized()
{

}
