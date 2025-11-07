#pragma once
#include <JuceHeader.h>

class WaveformDisplay : public juce::Component,
                        public juce::ChangeListener,
                        public juce::Timer

{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManager);
    ~WaveformDisplay() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void loadURL(const juce::URL& audioURL);
    void setPositionRelative(double pos);
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
    void mouseDown(const juce::MouseEvent& event) override;
    std::function<void(double)> onPositionChange;

private:
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    double position;
    bool fileLoaded;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};