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

    void play();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    void setLooping(bool shouldLoop);

	double getLengthInSeconds() const;
    AudioMetadata getMetadata() const { return metadata; }

    bool isPlaying() const;
    
	juce::AudioTransportSource& getTransportSource() noexcept { return transportSource; }

private:
    void extractMetadata(const juce::File& file);
    
    double lastPosition = 0.0;
    AudioMetadata metadata;

    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
