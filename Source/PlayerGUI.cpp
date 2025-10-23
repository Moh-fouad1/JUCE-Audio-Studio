#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Buttons
    for (auto* btn : { &loadButton, &playPauseButton, &restartButton, &stopButton, &StartButton, &EndButton, &loopButton })
    {
        addAndMakeVisible(*btn);
        btn->addListener(this);
    }

    // Volume slider
    volumeSlider.setRange(1, 100, 1);
    volumeSlider.setValue(50);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
}


PlayerGUI::~PlayerGUI() {}

void PlayerGUI::resized()
{
    int buttonWidth = 70;
    int buttonHeight = 35;
    int spacing = 10;
    int x = 20;
    int y = 20;

    auto place = [&](juce::Button& btn)
        {
            btn.setBounds(x, y, buttonWidth, buttonHeight);
            x += buttonWidth + spacing;
        };

    place(loadButton);
    place(playPauseButton);
    place(restartButton);
    place(stopButton);
    place(StartButton);
    place(EndButton);
    place(loopButton);

    // Volume slider below buttons
    volumeSlider.setBounds(20, y + buttonHeight + 40, getWidth() - 40, 30);
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
    else if (button == &playPauseButton)
    {
        auto& transport = playerAudio.getTransportSource();

        if (transport.isPlaying())
        {
            double currentPos = transport.getCurrentPosition();
            transport.stop();
			transport.setPosition(currentPos); // maintain position
            playPauseButton.setButtonText("Play");
        }
        else
        {
            transport.start();                   
            playPauseButton.setButtonText("Pause");
        }

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
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue() / 100.0f);
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
