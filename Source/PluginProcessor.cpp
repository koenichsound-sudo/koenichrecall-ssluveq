#include "PluginProcessor.h"
#include "PluginEditor.h"
RecallProcessor::RecallProcessor(): AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),state(*this,nullptr,"UVEQ_STATE",layout()) {}
juce::AudioProcessorValueTreeState::ParameterLayout RecallProcessor::layout() {
 juce::AudioProcessorValueTreeState::ParameterLayout p;
 for(int i=0;i<12;++i) { const auto& s=uveq::specs[(size_t)i];
  if(uveq::discrete(i)) p.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{s.id,1},s.name,uveq::choices(i),(int)s.initial));
  else { auto r=i==8 ? uveq::hmfRange() : juce::NormalisableRange<float>(s.min,s.max);
   p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{s.id,1},s.name,r,s.initial,juce::AudioParameterFloatAttributes().withLabel(s.unit))); }
 }
 p.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"in",1},"IN",true));
 p.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"lmfFocus",1},"LMF Focus",false));
 p.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"hmfFocus",1},"HMF Focus",false)); return p;
}
bool RecallProcessor::isBusesLayoutSupported(const BusesLayout& l) const {
 return !l.getMainInputChannelSet().isDisabled() && l.getMainInputChannelSet()==l.getMainOutputChannelSet();
}
void RecallProcessor::getStateInformation(juce::MemoryBlock& b) { auto v=state.copyState(); v.setProperty("schemaVersion",1,nullptr); if(auto xml=v.createXml()) copyXmlToBinary(*xml,b); }
void RecallProcessor::setStateInformation(const void* d,int n) { if(auto xml=getXmlFromBinary(d,n)) if(xml->hasTagName(state.state.getType())) {auto v=juce::ValueTree::fromXml(*xml); if(v.isValid()) state.replaceState(v);} }
juce::AudioProcessorEditor* RecallProcessor::createEditor() { return new RecallEditor(*this); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new RecallProcessor(); }
