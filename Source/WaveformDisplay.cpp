#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManager)
    : thumbnailCache(5),
    thumbnail(512, formatManager, thumbnailCache),
    position(0),
    fileLoaded(false)
{
    thumbnail.addChangeListener(this);
    startTimer(40); // Update 25 times per second
}

WaveformDisplay::~WaveformDisplay()
{
    thumbnail.removeChangeListener(this);
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a1a));

    // Draw outline
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    if (fileLoaded)
    {
        // Draw the waveform
        g.setColour(juce::Colour(0xff404040));
        thumbnail.drawChannel(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 0, 1.0f);

        // Draw the played portion
        auto audioLength = thumbnail.getTotalLength();
        if (audioLength > 0)
        {
            auto playedArea = getLocalBounds().withWidth((int)(getWidth() * position));
            g.setColour(juce::Colour(0xff1db954)); 
            thumbnail.drawChannel(g, playedArea, 0.0, audioLength * position, 0, 1.0f);
        }

        // Draw playhead line
        g.setColour(juce::Colours::white);
        int playheadX = (int)(getWidth() * position);
        g.drawLine(playheadX, 0, playheadX, getHeight(), 2.0f);
    }
    else
    {
        // Draw placeholder text
        g.setColour(juce::Colours::grey);
        g.setFont(14.0f);
        g.drawText("No audio file loaded", getLocalBounds(), juce::Justification::centred, true);
    }
}

void WaveformDisplay::resized() {}

void WaveformDisplay::loadURL(const juce::URL& audioURL)
{
    thumbnail.clear();
    fileLoaded = false;

    juce::File file(audioURL.getLocalFile());
    if (file.existsAsFile())
    {
        thumbnail.setSource(new juce::FileInputSource(file));
        fileLoaded = true;
    }
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (position != pos)
    {
        position = pos;
        repaint();
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        repaint();
    }
}

void WaveformDisplay::timerCallback()
{
    repaint();
}

// *** ADD MOUSE HANDLING IMPLEMENTATION ***
void WaveformDisplay::mouseDown(const juce::MouseEvent& event)
{
    if (fileLoaded && onPositionChange)
    {
        double newPosition = (double)event.getPosition().getX() / getWidth();
        onPositionChange(newPosition);
    }
}