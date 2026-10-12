#include "PluginEditor.h"
#include <BinaryData.h>
void PanelLook::drawRotarySlider(juce::Graphics& g,int x,int y,int w,int h,float pos,float start,float end,juce::Slider& slider) {
 auto& knob=static_cast<PanelKnob&>(slider); const auto& s=uveq::specs[(size_t)knob.i];
 auto b=juce::Rectangle<float>((float)x,(float)y,(float)w,(float)h).reduced(5); auto c=b.getCentre();
 g.setColour(juce::Colour(0xff757a74)); g.fillEllipse(b);
 g.setColour(juce::Colour(0xffc3c7c0)); g.fillEllipse(b.reduced(9));
 for(int k=0;k<12;++k) {auto a=(float)k*juce::MathConstants<float>::twoPi/12; auto v=juce::Point<float>(std::sin(a),-std::cos(a)); g.setColour(juce::Colour(0xff858b82));g.drawLine({c+v*77.f,c+v*86.f},7);}
 auto face=b.reduced(18); auto colour=juce::Colour(s.colour);
 g.setGradientFill(juce::ColourGradient(colour.brighter(.12f),face.getTopLeft(),colour.darker(.17f),face.getBottomRight(),false));g.fillEllipse(face);
 g.setColour(colour.darker(.4f));g.drawEllipse(face,2);
 float a=start+pos*(end-start); auto v=juce::Point<float>(std::sin(a),-std::cos(a));
 g.setColour(knob.i==10?juce::Colour(0xff303631):juce::Colour(0xfffaf9f3));g.drawLine({c,c+v*67.f},12);
 if(slider.hasKeyboardFocus(false)) {g.setColour(juce::Colours::white.withAlpha(.8f));g.drawEllipse(b.reduced(1),2);}
}
void PanelLook::drawToggleButton(juce::Graphics& g,juce::ToggleButton& b,bool hover,bool down) {
 auto r=b.getLocalBounds().toFloat();g.setColour(juce::Colour(0xff1f2121));g.fillRoundedRectangle(r,10);
 auto on=b.getToggleState(); auto inner=r.reduced(on?10.f:7.f);
 g.setColour(on?juce::Colour(0xff91958f):juce::Colour(0xffc4c7c0)); if(hover||down)g.setColour(on?juce::Colour(0xffaaa9b4):juce::Colours::lightgrey);g.fillRoundedRectangle(inner,5);
 g.setColour(on?juce::Colour(0xff747971):juce::Colour(0xffe1e3dc));g.drawRoundedRectangle(inner.reduced(2),4,3);
 if(b.getButtonText().isNotEmpty()) {g.setColour(juce::Colour(0xff1e2220));g.setFont(juce::FontOptions(34,juce::Font::bold));g.drawText(b.getButtonText(),b.getLocalBounds(),juce::Justification::centred);}
}
void RecallEditor::Panel::paint(juce::Graphics& g) {
 g.drawImage(image,getLocalBounds().toFloat());
 // Correct the supplied artwork's 70 Hz label to the requested 60 Hz.
 g.drawImage(image,440,137,48,36,530,137,48,36);
 g.setColour(juce::Colour(0xfff5f5f1));g.setFont(juce::FontOptions(34,juce::Font::bold));g.drawText("60",440,137,48,36,juce::Justification::centred);
 bool on=processor.state.getRawParameterValue("in")->load()>.5f;
 g.setColour(on?juce::Colour(0xffb170ff):juce::Colour(0xff342354));g.fillEllipse(263,690,46,46);
 if(on) {g.setColour(juce::Colour(0xffd0a3ff));g.drawEllipse(267,694,38,38,2);}
 g.setColour(juce::Colour(0xff313438));g.fillRect(390,1963,390,61);
 g.setColour(juce::Colour(0xffeeeeea));g.setFont(juce::FontOptions(25,juce::Font::bold));g.drawText("KOENICH RECALL",390,1966,390,28,juce::Justification::centred);
 g.setFont(juce::FontOptions(17));g.drawText("SSL UltraViolet EQ - 1.0.1",390,1994,390,24,juce::Justification::centred);
}
RecallEditor::RecallEditor(RecallProcessor& p):AudioProcessorEditor(p),panel(p) {
 panel.image=juce::ImageCache::getFromMemory(BinaryData::frontpanel_png,BinaryData::frontpanel_pngSize);
 addAndMakeVisible(panel);panel.setSize(1156,2048);panel.setLookAndFeel(&look);
 for(int i=0;i<12;++i) {auto& s=uveq::specs[(size_t)i];auto& k=knobs[(size_t)i];k=std::make_unique<PanelKnob>(i);
  panel.addAndMakeVisible(*k);k->setName(s.name);k->setTitle(s.name);k->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);k->setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
  k->setBounds(s.x-92,s.y-92,184,184);k->setMouseDragSensitivity(250);k->setPopupDisplayEnabled(true,true,this);k->setWantsKeyboardFocus(true);
  float span=uveq::discrete(i)?45.f:150.f;
  k->setRotaryParameters(juce::degreesToRadians(360.f-span),juce::degreesToRadians(360.f+span),true);
  sliders[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.state,s.id,*k);
  k->setDoubleClickReturnValue(true,s.initial);
  k->textFromValueFunction=[i](double v) {return uveq::discrete(i)?uveq::choices(i)[juce::jlimit(0,3,(int)std::round(v))]:juce::String(v,2)+uveq::specs[(size_t)i].unit;};
  k->setTooltip(juce::String(s.name)+" — ziehen / Pfeiltasten; Doppelklick: Standardwert");
 }
 for(int i=0;i<3;++i) {auto& b=buttons[(size_t)i];panel.addAndMakeVisible(b);b.setName(uveq::toggleIDs[(size_t)i]);b.setTooltip(i==0?"IN — Recall-Zustand, Audio bleibt unverändert":"Focus — speichert nur den Schaltzustand");toggles[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.state,uveq::toggleIDs[(size_t)i],b);}
 buttons[0].setButtonText("IN");buttons[0].setBounds(934,665,157,93);buttons[1].setBounds(383,1393,92,93);buttons[2].setBounds(696,1393,92,93);
 setResizable(true,true);getConstrainer()->setFixedAspectRatio(1156.0/2048.0);setResizeLimits(347,615,867,1536);setSize(520,921);startTimerHz(20);
}
RecallEditor::~RecallEditor() {stopTimer();panel.setLookAndFeel(nullptr);}
void RecallEditor::resized() {panel.setTransform(juce::AffineTransform::scale((float)getWidth()/1156.f,(float)getHeight()/2048.f));}
