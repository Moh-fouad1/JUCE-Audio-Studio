#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel
{

public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    



    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
	
    // ListBoxModel implementation
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;


private:
    struct PlaylistItem
    {
        juce::File file;
        AudioMetadata metadata;
    };
    
    void loadPlaylistFile(int index);
    void updateMetadataDisplay();
    juce::String formatTime(double seconds);
    
    PlayerAudio playerAudio;
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Simple text buttons
    juce::TextButton playPauseButton{ "Play" };
    juce::TextButton prevButton{ "Prev" };
    juce::TextButton nextButton{ "Next" };
    juce::TextButton shuffleButton{ "Shuffle" };
    
    juce::Slider seekSlider{juce::Slider::LinearHorizontal, juce::Slider::NoTextBox};
    juce::Slider volumeSlider{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};

    // Speed slider
    juce::Slider speedSlider;
    juce::Label speedLabel;
    
    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton clearButton{ "Clear" };
    juce::ToggleButton loopToggle{ "Loop" };
    juce::ToggleButton muteToggle{ "Mute" };
    
    juce::Label timeLabel;
    juce::Label durationLabel;
    
    // Metadata display
    juce::Label metadataDisplay;
    
    // Playlist
    juce::ListBox playlistBox;
    juce::Array<PlaylistItem> playlist;
    int currentTrackIndex = -1;
    
    bool isShuffling = false;
    juce::Array<int> shuffledIndices;
    float lastVolumeBeforeMute = 0.5f;

    double markerA = -1.0;
    double markerB = -1.0;
    juce::TextButton setAButton{ "Set A" };
    juce::TextButton setBButton{ "Set B" };
    juce::TextButton clearABButton{ "Clear A-B" };

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
