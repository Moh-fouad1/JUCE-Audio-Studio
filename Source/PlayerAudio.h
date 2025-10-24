#pragma once
#include <JuceHeader.h>

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

    bool isPlaying() const;
    
	juce::AudioTransportSource& getTransportSource() noexcept { return transportSource; } //added accessor for transport source


private:
    double lastPosition = 0.0;

    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
