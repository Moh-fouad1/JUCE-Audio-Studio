#include <JuceHeader.h>

class PlayerAudio {
public:

    PlayerAudio();
	~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
	
    bool LoadFile(const juce::File& file);
	void play();
	void stop();
	void setGain(float gain);
	void setPosition(double pos);
    double getlength() const;

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};