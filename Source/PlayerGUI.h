#pragma once
#include <JuceHeader.h>
#include "WaveformDisplay.h"
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel
{

public:
    PlayerGUI(const juce::String& sessionIdentifier);
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

    juce::AudioSource* getAudioSource() noexcept { return playerAudio.getAudioSource(); }
    void setExternalGain(float gain) { playerAudio.setExternalGain(gain); }

private:
    struct PlaylistItem
    {
        juce::File file;
        AudioMetadata metadata;
    };

    struct Marker
    {
        double time = 0.0;
        juce::String label;
    };

    class MarkerListModel : public juce::ListBoxModel
    {
    public:
        MarkerListModel(PlayerGUI& owner) : owner(owner) {}

        int getNumRows() override;
        void paintListBoxItem(int rowNumber, juce::Graphics&, int width, int height, bool rowIsSelected) override;
        void listBoxItemClicked(int row, const juce::MouseEvent&) override;
        void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;

    private:
        PlayerGUI& owner;
    };
    
    void loadPlaylistFile(int index);
    void updateMetadataDisplay();
    juce::String formatTime(double seconds);
    int findPlaylistIndex(const juce::File& file) const;
    void addToPlaylistIfNeeded(const juce::File& file, const AudioMetadata& metadata);
    void addMarker(double timeSeconds);
    void deleteSelectedMarker();
    void jumpToMarker(int index);
    const juce::Array<Marker>& getMarkers() const { return markers; }
    
    PlayerAudio playerAudio;
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Simple text buttons
    juce::TextButton playPauseButton{ "Play" };
    juce::TextButton prevButton{ "Prev" };
    juce::TextButton nextButton{ "Next" };
    juce::TextButton shuffleButton{ "Shuffle" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton goToStartButton{ "Go To Start" };
    juce::TextButton goToEndButton{ "Go To End" };
    juce::TextButton back10Button{ "-10s" };
    juce::TextButton forward10Button{ "+10s" };
    
    juce::Slider seekSlider{juce::Slider::LinearHorizontal, juce::Slider::NoTextBox};
    juce::Slider volumeSlider{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};

    // Speed slider
    juce::Slider speedSlider;
    juce::Label speedLabel;

    // Wave form
    WaveformDisplay waveformDisplay;

    
    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton clearButton{ "Clear" };
    juce::ToggleButton loopToggle{ "Loop" };
    juce::ToggleButton muteToggle{ "Mute" };
    juce::TextButton addMarkerButton{ "Add Marker" };
    juce::TextButton deleteMarkerButton{ "Delete Marker" };
    
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

    juce::Array<Marker> markers;
    MarkerListModel markerModel;
    juce::ListBox markersList;

    juce::File sessionFile;
    juce::File currentFile;
    bool isRestoringSession = false;
    juce::Time lastSessionSave;
    juce::String sessionId;

    void saveSession();
    void loadLastSession();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
