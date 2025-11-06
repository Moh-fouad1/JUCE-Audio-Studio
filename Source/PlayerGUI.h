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
    
    //new buttons 
    juce::TextButton playButton{ "►" };
    juce::TextButton pauseButton{ "⏸" };
    juce::TextButton StartButton{ "|◄" };
    juce::TextButton EndButton{ "►|" };
    juce::TextButton loopButton{ "loop" };
    bool isLooping = false;

    juce::Slider positionSlider;
    juce::Label timeLabel;

    double markerA = -1.0;
    double markerB = -1.0;
    juce::TextButton setAButton{ "Set A" };
    juce::TextButton setBButton{ "Set B" };
    juce::TextButton clearABButton{ "Clear A-B" };



    //adjustable volume slider
    juce::Slider volumeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
