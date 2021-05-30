#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :openButton("Open"),playButton("Play"),stopButton("Stop")
{
 
    setSize (200, 150);


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

    addAndMakeVisible(&openButton);

    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);
    playButton.onClick = [this] { playButtonClicked();};
    addAndMakeVisible(&playButton);

    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);
    stopButton.onClick = [this] {stopButtonClicked();};
    addAndMakeVisible(&stopButton);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
   
    shutdownAudio();
}

void MainComponent::playButtonClicked()
{

}

void MainComponent::stopButtonClicked()
{

}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
}

void MainComponent::openButtonClicked()
{
    juce::FileChooser chooser("Select Wav or AIFF",
        juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*wav,*aiff", true, false);

    if (chooser.browseForFileToOpen())
    {
        auto myFile = chooser.getResult();

        juce::AudioFormatReader* reader =formatManager.createReaderFor(myFile);

        if (reader != nullptr)
        {
            std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));

        }
    }
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
    openButton.setBounds(20, 10, getWidth() - 40, 30);
    playButton.setBounds(20, 50, getWidth() - 40, 30);
    stopButton.setBounds(20, 90, getWidth() - 40, 30);

}
