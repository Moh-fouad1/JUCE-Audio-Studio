#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
	

private:
    PlayerAudio playerAudio;
    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::TextButton loadButton{ "Load" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    
    //new buttons 
    juce::TextButton playPauseButton{ "Play" };
	juce::TextButton StartButton{ "Start" };
	juce::TextButton EndButton{ "End" };
    juce::TextButton loopButton{ "loop" };
    bool isLooping = false;
    
    
    //adjustable volume slider
    juce::Slider volumeSlider;  

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
