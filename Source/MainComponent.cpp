#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :juce::AudioAppComponent(otherDeviceManager),openButton("Open"),playButton("Play"),stopButton("Stop"),state(Stopped)
{
    otherDeviceManager.initialise(2, 2, nullptr, true);
    audioSettings.reset(
        new juce::AudioDeviceSelectorComponent(otherDeviceManager, 0, 2, 0, 2, true, true, true, true));
    addAndMakeVisible(audioSettings.get());
    setSize (200, 150);


    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
   
        setAudioChannels (2, 2);
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

    setSize(600, 600);
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
            stopButton.setEnabled(false);
            
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
            
        }
        else
        {
            transportStateChanged(Stopped);
            
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
    openButton.setBounds(20, 10, getWidth() - 30, 30);
    playButton.setBounds(20, 50, getWidth() - 30, 30);
    stopButton.setBounds(20, 90, getWidth() - 30, 30);
    audioSettings->setBounds(20,130,getWidth() - 20,30);
}
