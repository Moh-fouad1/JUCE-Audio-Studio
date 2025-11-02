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
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

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
    std::unique_ptr<juce::Drawable> createSVGIcon(const char* iconType);
    
    PlayerAudio playerAudio;
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Modern UI Components with SVG icons
    juce::DrawableButton playPauseButton{"", juce::DrawableButton::ImageFitted};
    std::unique_ptr<juce::Drawable> playIcon;
    std::unique_ptr<juce::Drawable> pauseIcon;
    
    juce::DrawableButton prevButton{"", juce::DrawableButton::ImageFitted};
    juce::DrawableButton nextButton{"", juce::DrawableButton::ImageFitted};
    juce::DrawableButton shuffleButton{"", juce::DrawableButton::ImageFitted};
    
    juce::Slider seekSlider{juce::Slider::LinearHorizontal, juce::Slider::NoTextBox};
    juce::Slider volumeSlider{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};
    
    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton clearButton{ "Clear" };
    
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
