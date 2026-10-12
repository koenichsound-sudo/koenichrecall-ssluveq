#pragma once
#include "PluginProcessor.h"
class PanelKnob final : public juce::Slider {
public:
 explicit PanelKnob(int index): i(index) {}
 double valueToProportionOfLength(double v) override { auto t=juce::Slider::valueToProportionOfLength(v); return uveq::specs[(size_t)i].reverse ? 1-t : t; }
 double proportionOfLengthToValue(double t) override { return juce::Slider::proportionOfLengthToValue(uveq::specs[(size_t)i].reverse ? 1-t : t); }
 int i;
};
class PanelLook final : public juce::LookAndFeel_V4 {
public:
 void drawRotarySlider(juce::Graphics&,int,int,int,int,float,float,float,juce::Slider&) override;
 void drawToggleButton(juce::Graphics&,juce::ToggleButton&,bool,bool) override;
};
class RecallEditor final : public juce::AudioProcessorEditor, private juce::Timer {
public:
 explicit RecallEditor(RecallProcessor&); ~RecallEditor() override;
 void resized() override;
private:
 struct Panel : juce::Component { explicit Panel(RecallProcessor& p):processor(p) {} void paint(juce::Graphics&) override; RecallProcessor& processor; juce::Image image; } panel;
 PanelLook look;
 std::array<std::unique_ptr<PanelKnob>,12> knobs;
 std::array<juce::ToggleButton,3> buttons;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,12> sliders;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>,3> toggles;
 juce::TooltipWindow tooltip {this,500};
 void timerCallback() override { panel.repaint(258,685,58,58); }
};
