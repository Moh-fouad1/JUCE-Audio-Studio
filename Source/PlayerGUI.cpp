#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(const juce::String& sessionIdentifier)
    : waveformDisplay(playerAudio.getFormatManager()),
      markerModel(*this),
      sessionId(sessionIdentifier)
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
    
    addAndMakeVisible(restartButton);
    restartButton.addListener(this);

    addAndMakeVisible(goToStartButton);
    goToStartButton.addListener(this);

    addAndMakeVisible(goToEndButton);
    goToEndButton.addListener(this);

    addAndMakeVisible(back10Button);
    back10Button.addListener(this);

    addAndMakeVisible(forward10Button);
    forward10Button.addListener(this);

    // Loop and Mute toggles
    addAndMakeVisible(loopToggle);
    addAndMakeVisible(muteToggle);
    loopToggle.addListener(this);
    muteToggle.addListener(this);

    addAndMakeVisible(addMarkerButton);
    addAndMakeVisible(deleteMarkerButton);
    addMarkerButton.addListener(this);
    deleteMarkerButton.addListener(this);

    markersList.setModel(&markerModel);
    markersList.setRowHeight(24);
    markersList.setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colour(0xff101010));
    markersList.setColour(juce::ListBox::ColourIds::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(markersList);

    // Wave form
    addAndMakeVisible(waveformDisplay);

    waveformDisplay.onPositionChange = [this](double newPosition)
        {
            auto& transport = playerAudio.getTransportSource();
            if (transport.getLengthInSeconds() > 0)
            {
                double newPosInSeconds = newPosition * transport.getLengthInSeconds();
                transport.setPosition(newPosInSeconds);
                seekSlider.setValue(newPosition, juce::dontSendNotification);
                saveSession();
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

    auto baseDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                       .getChildFile("SimpleAudioPlayer");
    baseDir.createDirectory();
    juce::String fileName = sessionId.isNotEmpty() ? (sessionId + "_lastSession.json") : juce::String("lastSession.json");
    sessionFile = baseDir.getChildFile(fileName);
    sessionFile.getParentDirectory().createDirectory();
    lastSessionSave = juce::Time::getCurrentTime();
    loadLastSession();
    
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
    juce::FontOptions fontOptions (16.0f, juce::Font::bold);
    juce::Font boldFont (fontOptions);
    metadataDisplay.setFont(boldFont);
    metadataDisplay.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(metadataDisplay);
    
    startTimer(100); // Update UI every 100ms
}

PlayerGUI::~PlayerGUI() 
{
    stopTimer();
    saveSession();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    // Modern dark theme background
    g.fillAll(juce::Colour(0xff0a0a0a));
    
    // Draw separator lines for visual organization (fixed positions based on layout)
    g.setColour(juce::Colour(0xff2a2a2a));
    // First separator after metadata/controls section (~110px)
    int firstSeparator = 110;
    // Second separator after playlist section (~330px total)
    int secondSeparator = 330;
    if (firstSeparator > 0 && firstSeparator < getHeight())
        g.fillRect(0, firstSeparator, getWidth(), 1);
    if (secondSeparator > 0 && secondSeparator < getHeight())
        g.fillRect(0, secondSeparator, getWidth(), 1);
}

void PlayerGUI::resized()
{
    auto area = getLocalBounds();
    const int padding = 15;
    area.reduce(padding, padding);
    
    // Top section: Metadata and controls (comfortable fixed size)
    const int topSectionHeight = 110;
    auto topSection = area.removeFromTop(topSectionHeight);
    
    // Metadata display
    auto metadataArea = topSection.removeFromTop(45).reduced(10, 5);
    metadataDisplay.setBounds(metadataArea);
    
    // Top controls (load/clear buttons)
    auto controlsArea = topSection.reduced(10, 10);
    const int buttonWidth = 120;
    loadButton.setBounds(controlsArea.removeFromLeft(buttonWidth));
    controlsArea.removeFromLeft(15);
    clearButton.setBounds(controlsArea.removeFromLeft(buttonWidth));
    
    // Playlist section (comfortable fixed size)
    const int playlistHeight = 220;
    auto playlistSection = area.removeFromTop(playlistHeight).reduced(10, 5);
    playlistBox.setBounds(playlistSection);
    
    // Bottom section: Player controls
    area.reduce(10, 10);

    // Waveform (comfortable fixed size)
    const int waveformHeight = 100;
    auto waveformSection = area.removeFromTop(waveformHeight).reduced(10, 5);
    waveformDisplay.setBounds(waveformSection);

    // Seek slider with time labels
    const int seekHeight = 40;
    auto seekArea = area.removeFromTop(seekHeight);
    const int labelWidth = 70;
    timeLabel.setBounds(seekArea.removeFromLeft(labelWidth));
    seekArea.removeFromLeft(10);
    durationLabel.setBounds(seekArea.removeFromRight(labelWidth));
    seekArea.removeFromLeft(10).removeFromRight(10);
    seekSlider.setBounds(seekArea);

    // Transport button row
    const int buttonRowHeight = 50;
    auto buttonRow = area.removeFromTop(buttonRowHeight).reduced(10, 0);
    const int buttonH = 38;
    const int spacing = 10;

    // Volume slider on the right
    const int volumeWidth = 220;
    auto volumeArea = buttonRow.removeFromRight(volumeWidth);
    volumeSlider.setBounds(volumeArea.getX() + 15,
                           volumeArea.getCentreY() - buttonH / 2,
                           volumeArea.getWidth() - 30,
                           buttonH);

    // Transport buttons with comfortable sizes
    int x = buttonRow.getX();
    int y = buttonRow.getY() + (buttonRow.getHeight() - buttonH) / 2;
    
    auto placeTransport = [&](juce::Button& b, int w)
    {
        b.setBounds(x, y, w, buttonH);
        x += w + spacing;
    };

    placeTransport(prevButton, 75);
    placeTransport(playPauseButton, 95);
    placeTransport(nextButton, 75);
    placeTransport(restartButton, 95);
    placeTransport(shuffleButton, 95);

    // Navigation / toggle row
    const int navRowHeight = 45;
    auto navRow = area.removeFromTop(navRowHeight).reduced(10, 0);
    
    // Speed control on the right
    const int speedWidth = 240;
    auto speedArea = navRow.removeFromRight(speedWidth);
    speedLabel.setBounds(speedArea.getX(), speedArea.getCentreY() - 16, 60, 32);
    speedSlider.setBounds(speedArea.getX() + 70, speedArea.getCentreY() - 16, speedArea.getWidth() - 80, 32);

    // Navigation buttons with comfortable sizes
    x = navRow.getX();
    y = navRow.getCentreY() - 16;
    
    auto placeNav = [&](juce::Component& comp, int w)
    {
        comp.setBounds(x, y, w, 32);
        x += w + spacing;
    };

    placeNav(goToStartButton, 105);
    placeNav(goToEndButton, 95);
    placeNav(back10Button, 85);
    placeNav(forward10Button, 85);
    placeNav(loopToggle, 85);
    placeNav(muteToggle, 85);
    placeNav(setAButton, 65);
    placeNav(setBButton, 65);
    placeNav(clearABButton, 95);

    // Marker controls
    const int markerControlHeight = 35;
    auto markerControls = area.removeFromTop(markerControlHeight).reduced(10, 5);
    const int markerButtonWidth = 130;
    addMarkerButton.setBounds(markerControls.removeFromLeft(markerButtonWidth));
    markerControls.removeFromLeft(15);
    deleteMarkerButton.setBounds(markerControls.removeFromLeft(markerButtonWidth));

    // Markers list (comfortable fixed size)
    const int markersHeight = 150;
    auto markersArea = area.removeFromTop(markersHeight).reduced(10, 5);
    markersList.setBounds(markersArea);
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
                        if (playerAudio.loadFile(file))
                        {
                            auto metadata = playerAudio.getMetadata();
                            addToPlaylistIfNeeded(file, metadata);
                            int idx = findPlaylistIndex(file);
                            if (idx >= 0)
                            {
                                currentTrackIndex = idx;
                                loadPlaylistFile(idx);
                            }
                        }
                    }
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
        currentFile = juce::File();
        sessionFile.deleteFile();
        markers.clear();
        markersList.updateContent();
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
            
            // Shuffle using fisher yates algorithm
            for (int i = shuffledIndices.size() - 1; i > 0; --i)
            {
                int j = rand() % (i + 1);
                // Manual swap for juce Array
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
        saveSession();
    }
    else if (button == &restartButton)
    {
        auto& transport = playerAudio.getTransportSource();
        transport.setPosition(0.0);
        transport.start();
        playPauseButton.setButtonText("Pause");
        seekSlider.setValue(0.0, juce::dontSendNotification);
        saveSession();
    }
    else if (button == &goToStartButton)
    {
        auto& transport = playerAudio.getTransportSource();
        transport.setPosition(0.0);
        seekSlider.setValue(0.0, juce::dontSendNotification);
        saveSession();
    }
    else if (button == &goToEndButton)
    {
        auto& transport = playerAudio.getTransportSource();
        double length = transport.getLengthInSeconds();
        if (length > 0.0)
        {
            transport.setPosition(juce::jmax(0.0, length - 0.1));
            seekSlider.setValue(1.0, juce::dontSendNotification);
        }
        saveSession();
    }
    else if (button == &back10Button)
    {
        auto& transport = playerAudio.getTransportSource();
        double newPos = juce::jmax(0.0, transport.getCurrentPosition() - 10.0);
        transport.setPosition(newPos);
        double length = transport.getLengthInSeconds();
        if (length > 0.0)
            seekSlider.setValue(newPos / length, juce::dontSendNotification);
        saveSession();
    }
    else if (button == &forward10Button)
    {
        auto& transport = playerAudio.getTransportSource();
        double length = transport.getLengthInSeconds();
        double newPos = transport.getCurrentPosition() + 10.0;
        if (length > 0.0)
            newPos = juce::jmin(length, newPos);
        transport.setPosition(newPos);
        if (length > 0.0)
            seekSlider.setValue(newPos / length, juce::dontSendNotification);
        saveSession();
    }
    else if (button == &loopToggle)
    {
        bool shouldLoop = loopToggle.getToggleState();
        playerAudio.setLooping(shouldLoop);
        saveSession();
    }
    else if (button == &muteToggle)
    {
        bool shouldMute = muteToggle.getToggleState();
        if (shouldMute)
        {
            lastVolumeBeforeMute = (float) volumeSlider.getValue() / 100.0f;
            playerAudio.setUserGain(0.0f);
        }
        else
        {
            playerAudio.setUserGain(lastVolumeBeforeMute);
        }
        saveSession();
    }
    else if (button == &addMarkerButton)
    {
        auto& transport = playerAudio.getTransportSource();
        if (currentFile.existsAsFile() && transport.getLengthInSeconds() > 0.0)
            addMarker(transport.getCurrentPosition());
    }
    else if (button == &deleteMarkerButton)
    {
        deleteSelectedMarker();
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
        float gain = (float)slider->getValue() / 100.0f;
        lastVolumeBeforeMute = gain;
        playerAudio.setUserGain(muteToggle.getToggleState() ? 0.0f : gain);
        saveSession();
    }
    else if (slider == &seekSlider)
    {
        auto& transport = playerAudio.getTransportSource();
        if (transport.getLengthInSeconds() > 0)
        {
            double newPos = slider->getValue() * transport.getLengthInSeconds();
            transport.setPosition(newPos);
            saveSession();
        }
    }
    else if (slider == &speedSlider)
    {
        playerAudio.setPlaybackSpeed((float)speedSlider.getValue());
        saveSession();
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

        if (!isRestoringSession && currentFile.existsAsFile())
        {
            auto now = juce::Time::getCurrentTime();
            if (now - lastSessionSave > juce::RelativeTime::seconds(1))
                saveSession();
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
            if (!isRestoringSession)
            {
                markers.clear();
                markersList.updateContent();
            }
            currentFile = item.file;
            currentTrackIndex = index;
            playerAudio.setPlaybackSpeed((float)speedSlider.getValue());
            playerAudio.setLooping(loopToggle.getToggleState());
            waveformDisplay.loadURL(juce::URL(item.file));
            seekSlider.setValue(0.0, juce::dontSendNotification);
            float gain = (float)volumeSlider.getValue() / 100.0f;
            lastVolumeBeforeMute = gain;
            playerAudio.setUserGain(muteToggle.getToggleState() ? 0.0f : gain);
            updateMetadataDisplay();
            playlistBox.selectRow(index);
            if (!isRestoringSession)
                saveSession();
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

int PlayerGUI::findPlaylistIndex(const juce::File& file) const
{
    for (int i = 0; i < playlist.size(); ++i)
    {
        if (playlist[i].file == file)
            return i;
    }
    return -1;
}

void PlayerGUI::addToPlaylistIfNeeded(const juce::File& file, const AudioMetadata& metadata)
{
    if (findPlaylistIndex(file) == -1)
    {
        PlaylistItem item;
        item.file = file;
        item.metadata = metadata;
        playlist.add(item);
        playlistBox.updateContent();
    }
}

void PlayerGUI::addMarker(double timeSeconds)
{
    Marker marker;
    marker.time = timeSeconds;
    marker.label = "Marker " + juce::String(markers.size() + 1) + " (" + formatTime(timeSeconds) + ")";
    markers.add(marker);
    markersList.updateContent();
    markersList.selectRow(markers.size() - 1);
    saveSession();
}

void PlayerGUI::deleteSelectedMarker()
{
    int selected = markersList.getSelectedRow();
    if (selected >= 0 && selected < markers.size())
    {
        markers.remove(selected);
        for (int i = 0; i < markers.size(); ++i)
            markers.getReference(i).label = "Marker " + juce::String(i + 1) + " (" + formatTime(markers[i].time) + ")";
        markersList.updateContent();
        saveSession();
    }
}

void PlayerGUI::jumpToMarker(int index)
{
    if (index >= 0 && index < markers.size())
    {
        auto& transport = playerAudio.getTransportSource();
        double length = transport.getLengthInSeconds();
        double target = markers[index].time;
        if (length > 0.0)
        {
            transport.setPosition(juce::jlimit(0.0, length, target));
            seekSlider.setValue(target / length, juce::dontSendNotification);
            saveSession();
        }
    }
}

int PlayerGUI::MarkerListModel::getNumRows()
{
    return owner.markers.size();
}

void PlayerGUI::MarkerListModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colour(0xff1db954));
    else if (rowNumber % 2 == 0)
        g.fillAll(juce::Colour(0xff181818));
    else
        g.fillAll(juce::Colour(0xff121212));

    if (rowNumber < owner.markers.size())
    {
        auto& marker = owner.markers[rowNumber];
        g.setColour(juce::Colours::white);
        g.setFont(14.0f);
        juce::String text = marker.label;
        g.drawText(text, 8, 0, width - 16, height, juce::Justification::centredLeft);
    }
}

void PlayerGUI::MarkerListModel::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    owner.jumpToMarker(row);
}

void PlayerGUI::MarkerListModel::listBoxItemDoubleClicked(int row, const juce::MouseEvent&)
{
    owner.jumpToMarker(row);
}

void PlayerGUI::saveSession()
{
    if (isRestoringSession || !currentFile.existsAsFile())
        return;

    juce::DynamicObject::Ptr root = new juce::DynamicObject();
    root->setProperty("file", currentFile.getFullPathName());
    root->setProperty("position", playerAudio.getTransportSource().getCurrentPosition());
    root->setProperty("speed", speedSlider.getValue());
    root->setProperty("loop", loopToggle.getToggleState());
    root->setProperty("mute", muteToggle.getToggleState());
    root->setProperty("volume", volumeSlider.getValue());

    juce::String markersData;
    for (const auto& marker : markers)
    {
        juce::String label = marker.label;
        label = label.replaceCharacter('|', '-').replaceCharacter(';', ' ');
        markersData += juce::String(marker.time, 6) + "|" + label + ";";
    }
    root->setProperty("markers", markersData);

    juce::var json(root.get());
    sessionFile.getParentDirectory().createDirectory();
    sessionFile.replaceWithText(juce::JSON::toString(json));
    lastSessionSave = juce::Time::getCurrentTime();
}

void PlayerGUI::loadLastSession()
{
    if (!sessionFile.existsAsFile())
        return;

    isRestoringSession = true;
    auto content = sessionFile.loadFileAsString();
    juce::var parsed = juce::JSON::parse(content);
    if (auto* obj = parsed.getDynamicObject())
    {
        auto getVar = [obj](const char* name) -> juce::var
        {
            const juce::Identifier id(name);
            return obj->hasProperty(id) ? obj->getProperty(id) : juce::var();
        };

        juce::File file(getVar("file").toString());

        auto varPosition = getVar("position");
        double position = varPosition.isDouble() || varPosition.isInt() ? (double)varPosition : 0.0;

        auto varSpeed = getVar("speed");
        double speed = varSpeed.isDouble() || varSpeed.isInt() ? (double)varSpeed : 1.0;

        auto varVolume = getVar("volume");
        double volume = varVolume.isDouble() || varVolume.isInt() ? (double)varVolume : 50.0;

        auto varLoop = getVar("loop");
        bool loopState = varLoop.isBool() ? (bool)varLoop : false;

        auto varMute = getVar("mute");
        bool muteState = varMute.isBool() ? (bool)varMute : false;

        if (file.existsAsFile() && playerAudio.loadFile(file))
        {
            currentFile = file;
            auto metadata = playerAudio.getMetadata();
            addToPlaylistIfNeeded(file, metadata);
            waveformDisplay.loadURL(juce::URL(file));
            playerAudio.setPlaybackSpeed((float)speed);
            speedSlider.setValue(speed, juce::dontSendNotification);
            volumeSlider.setValue(volume, juce::dontSendNotification);
            loopToggle.setToggleState(loopState, juce::dontSendNotification);
            playerAudio.setLooping(loopState);

            muteToggle.setToggleState(muteState, juce::dontSendNotification);
            lastVolumeBeforeMute = (float)(volume / 100.0f);
            if (muteState)
                playerAudio.setUserGain(0.0f);
            else
                playerAudio.setUserGain(lastVolumeBeforeMute);

            auto& transport = playerAudio.getTransportSource();
            double length = transport.getLengthInSeconds();
            if (length > 0.0)
            {
                transport.setPosition(juce::jlimit(0.0, length, position));
                seekSlider.setValue(position / length, juce::dontSendNotification);
            }

            updateMetadataDisplay();

            markers.clear();
            auto markersString = getVar("markers").toString();
            if (markersString.isNotEmpty())
            {
                juce::StringArray entries;
                entries.addTokens(markersString, ";", "");
                entries.trim();
                entries.removeEmptyStrings();
                for (auto& entry : entries)
                {
                    auto timeStr = entry.upToFirstOccurrenceOf("|", false, false);
                    auto labelPart = entry.fromFirstOccurrenceOf("|", false, false);
                    if (timeStr.isNotEmpty())
                    {
                        Marker marker;
                        marker.time = timeStr.getDoubleValue();
                        marker.label = labelPart.isNotEmpty() ? labelPart : (juce::String("Marker ") + juce::String(markers.size() + 1) + " (" + formatTime(marker.time) + ")");
                        markers.add(marker);
                    }
                }
                if (markers.size() > 0)
                    markersList.selectRow(0);
            }
            markersList.updateContent();

            int idx = findPlaylistIndex(file);
            if (idx >= 0)
            {
                playlistBox.selectRow(idx);
                currentTrackIndex = idx;
            }
        }
    }
    isRestoringSession = false;
}
