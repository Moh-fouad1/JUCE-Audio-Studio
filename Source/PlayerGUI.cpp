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
    addAndMakeVisible(volumeSlider);

    loadButton.addListener(this);
    restartButton.addListener(this);
    stopButton.addListener(this);
    playButton.addListener(this);
    pauseButton.addListener(this);
    StartButton.addListener(this);
    EndButton.addListener(this);
    volumeSlider.addListener(this);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
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
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
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
