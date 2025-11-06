#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio() {}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (auto* reader = formatManager.createReaderFor(file))
    {
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
        
        // Extract metadata after loading
        extractMetadata(file);
        
        return true;
    }
    return false;
}

void PlayerAudio::extractMetadata(const juce::File& file)
{
    // Reset metadata
    metadata = AudioMetadata();
    
    // Store filename as fallback
    metadata.filename = file.getFileNameWithoutExtension();
    
    // Get duration
    metadata.duration = getLengthInSeconds();
    
    // Try to read metadata from the file using AudioFormatManager
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader != nullptr)
    {
        auto metadataValues = reader->metadataValues;
        
        // Try to get title (check if value is not empty)
        juce::String titleVal = metadataValues.getValue("title", "");
        if (titleVal.isEmpty())
            titleVal = metadataValues.getValue("Title", "");
        if (titleVal.isEmpty())
            titleVal = metadataValues.getValue("TITLE", "");
        if (titleVal.isNotEmpty())
            metadata.title = titleVal;
        
        // Try to get artist
        juce::String artistVal = metadataValues.getValue("artist", "");
        if (artistVal.isEmpty())
            artistVal = metadataValues.getValue("Artist", "");
        if (artistVal.isEmpty())
            artistVal = metadataValues.getValue("ARTIST", "");
        if (artistVal.isNotEmpty())
            metadata.artist = artistVal;
        
        // Try to get album
        juce::String albumVal = metadataValues.getValue("album", "");
        if (albumVal.isEmpty())
            albumVal = metadataValues.getValue("Album", "");
        if (albumVal.isEmpty())
            albumVal = metadataValues.getValue("ALBUM", "");
        if (albumVal.isNotEmpty())
            metadata.album = albumVal;
    }
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

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
    transportSource.setPosition(0.0);
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    transportSource.setLooping(shouldLoop);
}

double PlayerAudio::getLengthInSeconds() const
{
    if (readerSource)
    {
        auto totalSamples = readerSource->getTotalLength();
        double sampleRate = readerSource->getAudioFormatReader()->sampleRate;

        if (sampleRate > 0)
            return totalSamples / sampleRate;
    }

    return 0.0;
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}
