#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
    : waveformDisplay(playerAudio.getFormatManager()) 
    {
    // Initialize playlist
    playlistBox.setModel(this);
    playlistBox.setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colour(0xff1a1a1a));
    playlistBox.setColour(juce::ListBox::ColourIds::textColourId, juce::Colours::white);
    playlistBox.setColour(juce::ListBox::ColourIds::outlineColourId, juce::Colour(0xff2a2a2a));
    addAndMakeVisible(playlistBox);
    
    // Setup text buttons
    addAndMakeVisible(loadButton);
    addAndMakeVisible(clearButton);
    loadButton.addListener(this);
    clearButton.addListener(this);
    
    addAndMakeVisible(playPauseButton);
    playPauseButton.addListener(this);
    playPauseButton.setButtonText("Play");
    playPauseButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff1db954));
    playPauseButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    
    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);
    prevButton.addListener(this);
    nextButton.addListener(this);
    prevButton.setButtonText("Prev");
    nextButton.setButtonText("Next");
    
    addAndMakeVisible(shuffleButton);
    shuffleButton.addListener(this);
    shuffleButton.setButtonText("Shuffle");
    
    // Loop and Mute toggles
    addAndMakeVisible(loopToggle);
    addAndMakeVisible(muteToggle);
    loopToggle.addListener(this);
    muteToggle.addListener(this);

    // Wave form
    addAndMakeVisible(waveformDisplay);

    waveformDisplay.onPositionChange = [this](double newPosition)
        {
            auto& transport = playerAudio.getTransportSource();
            if (transport.getLengthInSeconds() > 0)
            {
                double newPosInSeconds = newPosition * transport.getLengthInSeconds();
                transport.setPosition(newPosInSeconds);
            }
        };
    
    // A-B loop controls
    addAndMakeVisible(setAButton);
    addAndMakeVisible(setBButton);
    addAndMakeVisible(clearABButton);
    setAButton.addListener(this);
    setBButton.addListener(this);
    clearABButton.addListener(this);
    
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
    volumeSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff404040));
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    volumeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::lightgrey);
    addAndMakeVisible(volumeSlider);

    // Speed slider
    speedSlider.setRange(0.5, 2.0, 0.1);
    speedSlider.setValue(1.0);
    speedSlider.setSkewFactor(0.5); // Makes the slider more sensitive at lower values
    speedSlider.addListener(this);
    speedSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff404040));
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffffa500)); // Orange color
    speedSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::lightgrey);
    speedSlider.setNumDecimalPlacesToDisplay(1);
    addAndMakeVisible(speedSlider);

    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
    speedLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(speedLabel);
    
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
}

PlayerGUI::~PlayerGUI() 
{
    stopTimer();
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
    auto area = getLocalBounds();
    
    // Top section: Metadata and controls
    auto topSection = area.removeFromTop(150);
    
    // Metadata display
    auto metadataArea = topSection.removeFromTop(40).reduced(20, 5);
    metadataDisplay.setBounds(metadataArea);
    
    // Top controls
    auto controlsArea = topSection.reduced(20, 10);
    
    loadButton.setBounds(controlsArea.removeFromLeft(100));
    controlsArea.removeFromLeft(10);
    clearButton.setBounds(controlsArea.removeFromLeft(100));
    
    // Playlist section (middle)
    auto playlistSection = area.removeFromTop(200);
    playlistSection.reduce(20, 0);
    playlistBox.setBounds(playlistSection);
    
    // Bottom section: Player controls
    area.reduce(20, 10);

    // waveform
    auto waveformSection = area.removeFromTop(80).reduced(20, 10);
    waveformDisplay.setBounds(waveformSection);

    // Seek slider with time labels
    auto seekArea = area.removeFromTop(40);
    timeLabel.setBounds(seekArea.removeFromLeft(50));
    seekArea.removeFromLeft(10);
    durationLabel.setBounds(seekArea.removeFromRight(50));
    seekArea.removeFromLeft(10).removeFromRight(10);
    seekSlider.setBounds(seekArea);

    // Button row with simple incremental placement (original style)
    auto buttonRow = area.removeFromTop(50).reduced(20, 0);
    int x = buttonRow.getX();
    int y = buttonRow.getY();
    const int buttonW = 80;
    const int buttonH = 35;
    const int spacing = 10;

    auto place = [&](juce::Button& b, int w) {
        b.setBounds(x, y, w, buttonH);
        x += w + spacing;
        };

    place(prevButton, 70);
    place(playPauseButton, 90);
    place(nextButton, 70);
    place(shuffleButton, 90);

    // Volume slider on the right with fixed width
    volumeSlider.setBounds(buttonRow.getRight() - 220, buttonRow.getY(), 200, buttonH);

    // Second row for toggles and speed
    auto secondRow = area.removeFromTop(40).reduced(20, 0);
    x = secondRow.getX();
    y = secondRow.getY();

    // Toggle buttons
    loopToggle.setBounds(x, y, 80, 30);
    x += 80 + spacing;
    muteToggle.setBounds(x, y, 80, 30);
    x += 80 + spacing;

    // A-B loop buttons
    setAButton.setBounds(x, y, 60, 30);
    x += 60 + 5;
    setBButton.setBounds(x, y, 60, 30);
    x += 60 + 5;
    clearABButton.setBounds(x, y, 80, 30);
    x += 80 + spacing;

    // Speed slider on the right of second row
    speedLabel.setBounds(secondRow.getRight() - 270, y, 50, 30);
    speedSlider.setBounds(secondRow.getRight() - 220, y, 200, 30);
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
            playPauseButton.setButtonText("Pause");
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
            playPauseButton.setButtonText("Pause");
        }
    }
    else if (button == &playPauseButton)
    {
        auto& transport = playerAudio.getTransportSource();

        if (transport.isPlaying())
        {
            double currentPos = transport.getCurrentPosition();
            transport.stop();
            transport.setPosition(currentPos);
            playPauseButton.setButtonText("Play");
        }
        else
        {
            transport.start();                   
            playPauseButton.setButtonText("Pause");
        }
    }
    else if (button == &shuffleButton)
    {
        isShuffling = !isShuffling;
        if (isShuffling)
        {
            shuffledIndices.clear();
            for (int i = 0; i < playlist.size(); ++i)
                shuffledIndices.add(i);
            
            // Shuffle using Fisher-Yates algorithm
            for (int i = shuffledIndices.size() - 1; i > 0; --i)
            {
                int j = rand() % (i + 1);
                // Manual swap for juce::Array
                int temp = shuffledIndices[i];
                shuffledIndices.set(i, shuffledIndices[j]);
                shuffledIndices.set(j, temp);
            }
            
            // Visual feedback
            shuffleButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff1db954));
        }
        else
        {
            shuffleButton.setColour(juce::TextButton::buttonColourId, juce::Colour());
        }
    }
    else if (button == &loopToggle)
    {
        bool shouldLoop = loopToggle.getToggleState();
        playerAudio.setLooping(shouldLoop);
    }
    else if (button == &muteToggle)
    {
        bool shouldMute = muteToggle.getToggleState();
        if (shouldMute)
        {
            lastVolumeBeforeMute = (float) volumeSlider.getValue() / 100.0f;
            playerAudio.setGain(0.0f);
        }
        else
        {
            playerAudio.setGain(lastVolumeBeforeMute);
        }
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
        markerA = -1.0;
        markerB = -1.0;
    }

}   

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue() / 100.0f);
    }
    else if (slider == &seekSlider)
    {
        auto& transport = playerAudio.getTransportSource();
        if (transport.getLengthInSeconds() > 0)
        {
            double newPos = slider->getValue() * transport.getLengthInSeconds();
            transport.setPosition(newPos);
        }
    }
    else if (slider == &speedSlider)
    {
        playerAudio.setPlaybackSpeed((float)speedSlider.getValue());
    }
}

void PlayerGUI::timerCallback()
{
    auto& transport = playerAudio.getTransportSource();
    
    if (transport.getLengthInSeconds() > 0)
    {
        double currentPos = transport.getCurrentPosition();
        double duration = transport.getLengthInSeconds();
        
        timeLabel.setText(formatTime(currentPos), juce::dontSendNotification);
        durationLabel.setText(formatTime(duration), juce::dontSendNotification);
        
        // Update seek slider
        seekSlider.setValue(currentPos / duration, juce::dontSendNotification);
        
        // Update waveform display
        waveformDisplay.setPositionRelative(currentPos / duration);

        // Update button state
        if (transport.isPlaying())
        {
            playPauseButton.setButtonText("Pause");
        }
        else
        {
            playPauseButton.setButtonText("Play");
        }
        
        // A-B looping
        if (markerA >= 0.0 && markerB > markerA)
        {
            if (currentPos >= markerB)
            {
                transport.setPosition(markerA);
            }
        }

        // Auto-next track
        if (!transport.isPlaying() && currentPos >= duration - 0.1 && playlist.size() > 0)
        {
            // Move to next track
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
        }
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
        playPauseButton.setButtonText("Pause");
    }
}

void PlayerGUI::loadPlaylistFile(int index)
{
    if (index >= 0 && index < playlist.size())
    {
        auto& item = playlist[index];
        if (playerAudio.loadFile(item.file))
        {
            waveformDisplay.loadURL(juce::URL(item.file));

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

// No icon creation needed for text buttons
