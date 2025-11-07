#pragma once
#include <JuceHeader.h>
#include "PlayerGUI.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    class ScrollableContent : public juce::Component
    {
    public:
        ScrollableContent(PlayerGUI& left, PlayerGUI& right) 
            : leftPlayer(left), rightPlayer(right) {}
        
        void resized() override
        {
            auto area = getLocalBounds();
            const int padding = 10;
            const int gap = 15;
            
            // Set preferred height for each player (comfortable size - calculated from PlayerGUI layout)
            const int preferredPlayerHeight = 800;
            
            // Top player
            leftPlayer.setBounds(area.removeFromTop(preferredPlayerHeight).reduced(padding, 0));
            
            // Gap between players
            area.removeFromTop(gap);
            
            // Bottom player
            rightPlayer.setBounds(area.removeFromTop(preferredPlayerHeight).reduced(padding, 0));
        }
        
    private:
        PlayerGUI& leftPlayer;
        PlayerGUI& rightPlayer;
    };
    
    PlayerGUI leftPlayer;
    PlayerGUI rightPlayer;
    ScrollableContent scrollableContent;
    juce::Viewport viewport;
    juce::Slider mixSlider;
    juce::Label mixLabel;
    juce::MixerAudioSource mixer;

    void updateMixGains();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
