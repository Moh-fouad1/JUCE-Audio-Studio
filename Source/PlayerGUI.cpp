#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Initialize playlist
    playlistBox.setModel(this);
    playlistBox.setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colour(0xff1a1a1a));
    playlistBox.setColour(juce::ListBox::ColourIds::textColourId, juce::Colours::white);
    playlistBox.setColour(juce::ListBox::ColourIds::outlineColourId, juce::Colour(0xff2a2a2a));
    addAndMakeVisible(playlistBox);
    
    // Create SVG icons
    playIcon = createSVGIcon("M8 5v14l11-7z");
    pauseIcon = createSVGIcon("M6 4h4v16H6zm8 0h4v16h-4z");
    
    auto prevIcon = createSVGIcon("M6 6h2v12H6zm3.5 6l8.5 6V6z");
    auto nextIcon = createSVGIcon("M15 6l-8.5 6 8.5 6V6zm3 0v12h-2V6h2z");
    auto shuffleIcon = createSVGIcon("M14.83 13.83l1.68-1.68-2.12-2.12-1.68 1.68zM10.59 8.41l4.24-4.24 2.12 2.12-4.24 4.24zM13.17 6.83l-1.68-1.68L14.49 4h-6V2h6l2.83 2.83L16.07 5v11l-2.83-2.83z");
    
    // Setup buttons with icons
    addAndMakeVisible(loadButton);
    addAndMakeVisible(clearButton);
    loadButton.addListener(this);
    clearButton.addListener(this);
    
    addAndMakeVisible(playPauseButton);
    playPauseButton.addListener(this);
    playPauseButton.setImages(playIcon.get());
    playPauseButton.setColour(juce::DrawableButton::backgroundColourId, juce::Colour(0xff1db954));
    playPauseButton.setColour(juce::DrawableButton::backgroundOnColourId, juce::Colour(0xff1db954));
    
    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);
    prevButton.addListener(this);
    nextButton.addListener(this);
    prevButton.setImages(prevIcon.get());
    nextButton.setImages(nextIcon.get());
    
    addAndMakeVisible(shuffleButton);
    shuffleButton.addListener(this);
    shuffleButton.setImages(shuffleIcon.get());
    
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff333333));
    loadButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    clearButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff333333));
    clearButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    
    // Seek slider
    seekSlider.setRange(0.0, 1.0);
    seekSlider.setValue(0.0);
    seekSlider.addListener(this);
    seekSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff404040));
    seekSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff1db954));
    addAndMakeVisible(seekSlider);

    // Volume slider
    volumeSlider.setRange(0, 100, 1);
    volumeSlider.setValue(50);
    volumeSlider.setTextValueSuffix("%");
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
    
    // Time labels
    timeLabel.setText("0:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centredLeft);
    timeLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(timeLabel);
    
    durationLabel.setText("0:00", juce::dontSendNotification);
    durationLabel.setJustificationType(juce::Justification::centredRight);
    durationLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(durationLabel);
    
    // Metadata display
    metadataDisplay.setText("No track loaded", juce::dontSendNotification);
    metadataDisplay.setJustificationType(juce::Justification::centredLeft);
    juce::Font boldFont = juce::Font(16.0f).boldened();
    metadataDisplay.setFont(boldFont);
    metadataDisplay.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(metadataDisplay);
    
    startTimer(100); // Update UI every 100ms
    addAndMakeVisible(muteButton);

    loadButton.addListener(this);
    restartButton.addListener(this);
    stopButton.addListener(this);
    playButton.addListener(this);
    pauseButton.addListener(this);
    StartButton.addListener(this);
    EndButton.addListener(this);
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

void PlayerGUI::paint(juce::Graphics& g)
{
    // Modern dark theme background
    g.fillAll(juce::Colour(0xff0a0a0a));
    
    // Draw separator lines for visual organization
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRect(0, 150, getWidth(), 2);
    g.fillRect(0, 420, getWidth(), 2);
}

void PlayerGUI::resized()
{
    int buttonWidth = 70;
    int buttonHeight = 35;
    int spacing = 10;
    int x = 20;
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(240, y, 80, 40);
    stopButton.setBounds(340, y, 80, 40);
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
            "Select audio files...", juce::File{}, "*.wav;*.mp3;*.aac;*.flac;*.ogg;*.m4a");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | 
            juce::FileBrowserComponent::canSelectMultipleItems |
            juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto files = fc.getResults();
                for (auto& file : files)
                {
                if (file.existsAsFile())
                    {
                        PlaylistItem item;
                        item.file = file;
                        
                        // Load metadata
                        if (playerAudio.loadFile(file))
                        {
                            item.metadata = playerAudio.getMetadata();
                            playlist.add(item);
                        }
                    }
                }
                
                playlistBox.updateContent();
                
                // Load first file if playlist was empty
                if (playlist.size() > 0 && currentTrackIndex == -1)
                {
                    currentTrackIndex = 0;
                    loadPlaylistFile(0);
                    auto& transportRef = playerAudio.getTransportSource();
                    transportRef.start();
                }
            });
    }
    else if (button == &clearButton)
    {
        playlist.clear();
        currentTrackIndex = -1;
        playlistBox.updateContent();
        metadataDisplay.setText("No track loaded", juce::dontSendNotification);
        
        auto& transport = playerAudio.getTransportSource();
        transport.stop();
        transport.setSource(nullptr);
    }
    else if (button == &prevButton)
    {
        if (playlist.size() > 0)
        {
            auto& transport = playerAudio.getTransportSource();
            if (isShuffling && shuffledIndices.size() > 0)
            {
                int shuffleIndex = 0;
                for (int i = 0; i < shuffledIndices.size(); ++i)
                {
                    if (shuffledIndices[i] == currentTrackIndex)
                    {
                        shuffleIndex = i;
                        break;
                    }
                }
                shuffleIndex = (shuffleIndex - 1 + shuffledIndices.size()) % shuffledIndices.size();
                currentTrackIndex = shuffledIndices[shuffleIndex];
            }
            else
            {
                currentTrackIndex = (currentTrackIndex - 1 + playlist.size()) % playlist.size();
            }
            loadPlaylistFile(currentTrackIndex);
            transport.start();
            playPauseButton.setImages(pauseIcon.get());
        }
    }
    else if (button == &nextButton)
    {
        if (playlist.size() > 0)
        {
            auto& transport = playerAudio.getTransportSource();
            if (isShuffling && shuffledIndices.size() > 0)
            {
                int shuffleIndex = 0;
                for (int i = 0; i < shuffledIndices.size(); ++i)
                {
                    if (shuffledIndices[i] == currentTrackIndex)
                    {
                        shuffleIndex = i;
                        break;
                    }
                }
                shuffleIndex = (shuffleIndex + 1) % shuffledIndices.size();
                currentTrackIndex = shuffledIndices[shuffleIndex];
            }
            else
            {
                currentTrackIndex = (currentTrackIndex + 1) % playlist.size();
            }
            loadPlaylistFile(currentTrackIndex);
            transport.start();
            playPauseButton.setImages(pauseIcon.get());
        }
    }
        playerAudio.stop();
        playerAudio.setPosition(0.0);
    }
    else if (button == &playPauseButton)
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
    else if (button == &muteButton)
    {
        if (!Mute) {
            playerAudio.setGain(0.0);
            Mute = true;
            muteButton.setButtonText("Unmute");
        }
        else if (Mute) {
            playerAudio.setGain(lastVolume);
            Mute = false;
            muteButton.setButtonText("Mute");
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

int PlayerGUI::getNumRows()
{
    return playlist.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colour(0xff1db954)); // Spotify green
    }
    else if (rowNumber % 2 == 0)
    {
        g.fillAll(juce::Colour(0xff1a1a1a));
    }
    else
    {
        g.fillAll(juce::Colour(0xff141414));
    }
    
    if (rowNumber < playlist.size())
    {
        auto& item = playlist[rowNumber];
        
        // Draw title
        g.setColour(juce::Colours::white);
        g.setFont(14.0f);
        
        juce::String displayText;
        if (item.metadata.title.isNotEmpty())
            displayText = item.metadata.title;
        else
            displayText = item.metadata.filename;
        
        g.drawText(displayText, 10, 0, width - 60, height / 2, juce::Justification::centredLeft);
        
        // Draw artist/album info
        g.setColour(juce::Colours::lightgrey);
        g.setFont(12.0f);
        
        juce::String info;
        if (item.metadata.artist.isNotEmpty())
            info = item.metadata.artist;
        if (item.metadata.album.isNotEmpty())
        {
            if (info.isNotEmpty())
                info += " - ";
            info += item.metadata.album;
        }
        
        g.drawText(info, 10, height / 2, width - 60, height / 2, juce::Justification::centredLeft);
        
        // Draw duration
        g.drawText(formatTime(item.metadata.duration), width - 60, 0, 50, height, juce::Justification::centredRight);
    }
}

void PlayerGUI::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    if (row >= 0 && row < playlist.size())
    {
        currentTrackIndex = row;
        loadPlaylistFile(row);
        
        auto& transport = playerAudio.getTransportSource();
        transport.start();
        playPauseButton.setImages(pauseIcon.get());
    }
}

void PlayerGUI::loadPlaylistFile(int index)
{
    if (index >= 0 && index < playlist.size())
    {
        auto& item = playlist[index];
        if (playerAudio.loadFile(item.file))
        {
            updateMetadataDisplay();
            playlistBox.selectRow(index);
        }
    }
}

void PlayerGUI::updateMetadataDisplay()
{
    auto metadata = playerAudio.getMetadata();
    
    juce::String displayText;
    
    if (metadata.title.isNotEmpty() || metadata.artist.isNotEmpty())
    {
        if (metadata.title.isNotEmpty())
            displayText = metadata.title;
        else
            displayText = metadata.filename;
        
        if (metadata.artist.isNotEmpty())
        {
            displayText += " - " + metadata.artist;
        }
    }
    else
    {
        displayText = metadata.filename;
    }
    
    metadataDisplay.setText(displayText, juce::dontSendNotification);
}

juce::String PlayerGUI::formatTime(double seconds)
{
    int mins = (int)(seconds / 60);
    int secs = (int)(seconds) % 60;
    return juce::String(mins) + ":" + (secs < 10 ? "0" : "") + juce::String(secs);
}

std::unique_ptr<juce::Drawable> PlayerGUI::createSVGIcon(const char* iconType)
{
    auto drawable = std::make_unique<juce::DrawablePath>();
    juce::Path path;
    
    // Create simple geometric icons
    juce::String type = juce::String::fromUTF8(iconType);
    
    if (type == "M8 5v14l11-7z") // Play icon
    {
        path.startNewSubPath(8, 5);
        path.lineTo(19, 12);
        path.lineTo(8, 19);
        path.closeSubPath();
    }
    else if (type == "M6 4h4v16H6zm8 0h4v16h-4z") // Pause icon
    {
        path.addRectangle(6, 4, 4, 16);
        path.addRectangle(14, 4, 4, 16);
    }
    else if (type == "M6 6h2v12H6zm3.5 6l8.5 6V6z") // Previous icon
    {
        path.addRectangle(6, 6, 2, 12);
        path.startNewSubPath(9.5f, 6);
        path.lineTo(9.5f, 18);
        path.lineTo(18, 12);
        path.closeSubPath();
    }
    else if (type == "M15 6l-8.5 6 8.5 6V6zm3 0v12h-2V6h2z") // Next icon
    {
        path.startNewSubPath(6.5f, 6);
        path.lineTo(15, 12);
        path.lineTo(6.5f, 18);
        path.closeSubPath();
        path.addRectangle(18, 6, 2, 12);
    }
    else if (type == "M14.83 13.83l1.68-1.68-2.12-2.12-1.68 1.68zM10.59 8.41l4.24-4.24 2.12 2.12-4.24 4.24zM13.17 6.83l-1.68-1.68L14.49 4h-6V2h6l2.83 2.83L16.07 5v11l-2.83-2.83z") // Shuffle icon
    {
        // Simplified shuffle icon - two arrows crossing
        path.startNewSubPath(10, 8);
        path.lineTo(14, 12);
        path.lineTo(10, 16);
        path.startNewSubPath(14, 8);
        path.lineTo(10, 12);
        path.lineTo(14, 16);
    }
    
    drawable->setPath(path);
    drawable->setFill(juce::Colours::white);
    
    return drawable;
}
