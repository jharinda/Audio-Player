#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :openButton("Open"),playButton("Play"),stopButton("Stop"),state(Stopped)
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
    transport.addChangeListener(this);
}

MainComponent::~MainComponent()
{
   
    shutdownAudio();
}

void MainComponent::playButtonClicked()
{
    transportStateChanged(Starting);
}

void MainComponent::stopButtonClicked()
{
    transportStateChanged(Stopping);
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transport.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
}

void MainComponent::openButtonClicked()
{
    juce::FileChooser chooser("Select Wav or AIFF",
        juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*wav;*aiff;*mp3", true, false);

    if (chooser.browseForFileToOpen())
    {
        auto myFile = chooser.getResult();

        juce::AudioFormatReader* reader =formatManager.createReaderFor(myFile);

        if (reader != nullptr)
        {
            std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
            
            transport.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

            readerSource.reset(newSource.release());
            playButton.setEnabled(true);
            
        }
    }
}

void MainComponent::transportStateChanged(TransportState newState)
{
    if (state != newState)
    {
        state = newState;

        switch (state)
        {
        case Stopped:
            transport.setPosition(0.0);
            playButton.setEnabled(true);
            stopButton.setEnabled(false);
            break;

        case Starting:
            /*playButton.setEnabled(false);*/
            stopButton.setEnabled(true);
            transport.start();
            break;

        case Stopping:
            /*playButton.setEnabled(true);
            stopButton.setEnabled(false);*/
            transport.stop();
            break;

        case Playing:
            playButton.setEnabled(false);
            /*stopButton.setEnabled(true);*/
            break;


        }
    }
}
void MainComponent::changeListenerCallback(juce::ChangeBroadcaster *source) 
{
    if (source == &transport)
    {
        if (transport.isPlaying())
        {
            transportStateChanged(Playing);
            DBG("isPlaying");
        }
        else
        {
            transportStateChanged(Stopped);
            DBG("Stopped");
        }
    }
}
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transport.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    transport.releaseResources();
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
