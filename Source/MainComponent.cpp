#include "MainComponent.h"
#include <cmath>

MainComponent::MainComponent()
    : leftPlayer("playerA"), 
      rightPlayer("playerB"),
      scrollableContent(leftPlayer, rightPlayer)
{
    setSize(1100, 760);

    // Setup viewport
    viewport.setViewedComponent(&scrollableContent, false);
    viewport.setScrollBarsShown(true, false); // Show vertical scrollbar, hide horizontal
    viewport.setScrollBarThickness(12);
    addAndMakeVisible(viewport);

    // Add players to scrollable content
    scrollableContent.addAndMakeVisible(leftPlayer);
    scrollableContent.addAndMakeVisible(rightPlayer);

    mixSlider.setRange(0.0, 1.0, 0.01);
    mixSlider.setValue(0.5);
    mixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    mixSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff404040));
    mixSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff1db954));
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    mixSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff2a2a2a));
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff404040));
    mixSlider.onValueChange = [this]() { updateMixGains(); };
    addAndMakeVisible(mixSlider);

    mixLabel.setText("Mix: Left <-> Right", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    juce::FontOptions fontOptions(16.0f, juce::Font::bold);
    juce::Font labelFont(fontOptions);
    mixLabel.setFont(labelFont);
    addAndMakeVisible(mixLabel);

    mixer.addInputSource(leftPlayer.getAudioSource(), false);
    mixer.addInputSource(rightPlayer.getAudioSource(), false);

    setAudioChannels(0, 2);
    updateMixGains();
}

MainComponent::~MainComponent()
{
    mixer.removeAllInputs();
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    leftPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    rightPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    updateMixGains();
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    updateMixGains();
    mixer.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    mixer.releaseResources();
    leftPlayer.releaseResources();
    rightPlayer.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0a0a0a));
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    
    // Mix controls at the bottom (fixed, not scrollable) - more space for visibility
    const int mixControlHeight = 90;
    auto mixArea = area.removeFromBottom(mixControlHeight);
    
    // Center the mix controls with proper spacing
    mixArea.reduce(50, 10); // Less horizontal reduction for more slider space
    
    mixLabel.setBounds(mixArea.removeFromTop(25));
    mixArea.removeFromTop(5); // Small gap between label and slider
    mixSlider.setBounds(mixArea); // Full width of reduced area
    
    // Viewport takes remaining space
    viewport.setBounds(area);
    
    // Update scrollable content size
    const int preferredPlayerHeight = 800;
    const int gap = 15;
    int contentWidth = area.getWidth();
    int contentHeight = preferredPlayerHeight + gap + preferredPlayerHeight;
    scrollableContent.setSize(contentWidth, contentHeight);
}

void MainComponent::updateMixGains()
{
    float mix = (float) mixSlider.getValue();
    float leftValue = juce::jlimit(0.0f, 1.0f, 1.0f - mix);
    float rightValue = juce::jlimit(0.0f, 1.0f, mix);
    float leftGain = std::sqrt(leftValue);
    float rightGain = std::sqrt(rightValue);
    leftPlayer.setExternalGain(leftGain);
    rightPlayer.setExternalGain(rightGain);
}
