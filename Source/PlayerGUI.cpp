#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &restartButton, &stopButton })
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

    volumeSlider.setRange(1, 100, 1);
    volumeSlider.setValue(50);
    volumeSlider.addListener(this);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(restartButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(StartButton);
    addAndMakeVisible(EndButton);
    addAndMakeVisible(loopButton);
    loopButton.addListener(this);
    addAndMakeVisible(volumeSlider);

    loadButton.addListener(this);
    restartButton.addListener(this);
    stopButton.addListener(this);
    playButton.addListener(this);
    pauseButton.addListener(this);
    StartButton.addListener(this);
    EndButton.addListener(this);
    loopButton.addListener(this);
    volumeSlider.addListener(this);

    positionSlider.setRange(0.0, 1.0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(timeLabel);

    addAndMakeVisible(setAButton);
    addAndMakeVisible(setBButton);
    addAndMakeVisible(clearABButton);

    setAButton.addListener(this);
    setBButton.addListener(this);
    clearABButton.addListener(this);

    startTimerHz(30);


}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    loopButton.setBounds(350, y, 100, 40);
    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
    positionSlider.setBounds(20, 150, getWidth() - 40, 30);
    timeLabel.setBounds(20, 185, 200, 25);

    setAButton.setBounds(20, 220, 90, 35);
    setBButton.setBounds(120, 220, 90, 35);
    clearABButton.setBounds(220, 220, 120, 35);

}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...", juce::File{}, "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                    playerAudio.loadFile(file);
            });
    }
    else if (button == &restartButton)
    {
        playerAudio.setPosition(0.0);
        playerAudio.play();
    }
    else if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
    }
    else if (button == &playButton)
    {
        playerAudio.play();
    }
    else if (button == &pauseButton)
    {
        playerAudio.stop(); // wont reset position, just pauses
    }
    else if (button == &StartButton)
    {
        playerAudio.setPosition(0.0);
    }
    else if (button == &EndButton)
    {
        double length = playerAudio.getLengthInSeconds();
        if (length > 0.0)
            playerAudio.setPosition(length);
    }
    else if (button == &loopButton)
    {
        isLooping = !isLooping;
        playerAudio.setLooping(isLooping);
        loopButton.setButtonText(isLooping ? "unloop" : "loop");
    }
    else if (button == &setAButton)
    {
        markerA = playerAudio.getCurrentPosition();
    }
    else if (button == &setBButton)
    {
        markerB = playerAudio.getCurrentPosition();
    }
    else if (button == &clearABButton)
    {
        markerA = markerB = -1.0;
    }

}
void PlayerGUI::timerCallback()
{
    double current = playerAudio.getCurrentPosition();
    double length = playerAudio.getLengthInSeconds();

    if (length > 0.0)
    {
        positionSlider.setValue(current / length, juce::dontSendNotification);

        int sec = (int)current % 60;
        int min = (int)(current / 60);
        timeLabel.setText(juce::String(min) + ":" + juce::String(sec).paddedLeft('0', 2),
            juce::dontSendNotification);
    }

    if (markerA >= 0 && markerB > markerA)
    {
        if (current >= markerB)
            playerAudio.setPosition(markerA);
    }
}


void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
    else if (slider == &positionSlider)
    {
        double len = playerAudio.getLengthInSeconds();
        playerAudio.setPosition(len * positionSlider.getValue());
    }

}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}
