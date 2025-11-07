#pragma once
#include <JuceHeader.h>

// Structure to hold metadata
struct AudioMetadata
{
    juce::String title;
    juce::String artist;
    juce::String album;
    juce::String filename;
    double duration = 0.0;
};

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    bool loadFile(const juce::File& file);

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);

    void releaseResources();

    // for the waveform
    juce::AudioFormatManager& getFormatManager() { return formatManager; }


    void play();
    void stop();
    void setUserGain(float gain);
    void setExternalGain(float gain);
    float getUserGain() const { return userGain; }
    float getExternalGain() const { return externalGain; }
    void setPosition(double pos);
    void setLooping(bool shouldLoop);
    double getCurrentPosition() const { return transportSource.getCurrentPosition(); }

    void setPlaybackSpeed(float speed);
    float getPlaybackSpeed() const { return playbackSpeed; }

	double getLengthInSeconds() const;
    AudioMetadata getMetadata() const { return metadata; }

    bool isPlaying() const;

    juce::AudioTransportSource& getTransportSource() noexcept { return transportSource; }
    juce::AudioSource* getAudioSource() noexcept { return resamplingSource.get(); }

private:
    void extractMetadata(const juce::File& file);
    void updateGain();
    
    double lastPosition = 0.0;
    AudioMetadata metadata;

    float playbackSpeed = 1.0f;
    std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;

    float userGain = 0.5f;
    float externalGain = 1.0f;

    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
