#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
	formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
	releaseResources();
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	transportSource.getNextAudioBlock(bufferToFill);
}
void PlayerAudio::releaseResources()
{
	transportSource.releaseResources();
}
bool PlayerAudio::LoadFile(const juce::File& file) {

    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
           
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

           
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);
            transportSource.start();
        }
    }
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}


