#include "../Source/PluginProcessor.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
static void require(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
template<class T> void audioTest(RecallProcessor& p,int channels,int samples) {
 juce::AudioBuffer<T> b(channels,samples), original(channels,samples);
 for(int c=0;c<channels;++c)for(int n=0;n<samples;++n)b.setSample(c,n,(T)(std::sin(n*0.137+c)*0.91));
 original.makeCopyOf(b);juce::MidiBuffer midi;p.processBlock(b,midi);
 for(int c=0;c<channels;++c) require(std::memcmp(b.getReadPointer(c),original.getReadPointer(c),sizeof(T)*(size_t)samples)==0,"Audio changed");
}
int main(int argc,char** argv){
 juce::ScopedJuceInitialiser_GUI gui;
 try {
 RecallProcessor a,b;
 for(int i : {1,3,10}) { auto* p=a.state.getParameter(uveq::specs[(size_t)i].id); require(p->getNumSteps()==4,"Discrete step count"); for(int j=0;j<4;++j) {p->setValueNotifyingHost((float)j/3);require(p->getCurrentValueAsText()==uveq::choices(i)[j],"Choice labels");} }
 for(int i : {6,9}) require(uveq::specs[(size_t)i].reverse,"Q direction");
 auto range=uveq::hmfRange(); require(std::abs(range.convertFrom0to1(.5f)-3.f)<.0001f,"HMF midpoint");
 for(int n=0;n<=1000;++n){float t=(float)n/1000;require(std::abs(range.convertTo0to1(range.convertFrom0to1(t))-t)<.00001f,"HMF mapping inverse");}
 for(auto* id : {"lmfFocus","hmfFocus","in"}) { auto before=a.state.copyState(); a.state.getParameter(id)->setValueNotifyingHost(1); for(const auto& s:uveq::specs) require((float)before.getChildWithProperty("id",s.id).getProperty("value")==a.state.getRawParameterValue(s.id)->load(),"Switch changed knob"); }
 require(a.getParameters().size()==15,"Parameter count");
 for(auto* p:a.getParameters())p->setValueNotifyingHost(p->getParameterIndex()>=12?1.0f:(float)(p->getParameterIndex()+1)/15.0f);
 juce::MemoryBlock block;a.getStateInformation(block);b.setStateInformation(block.getData(),(int)block.getSize());
 for(int i=0;i<15;++i){if(std::abs(a.getParameters()[i]->getValue()-b.getParameters()[i]->getValue())>=0.0001f)std::cerr<<i<<": "<<a.getParameters()[i]->getValue()<<" vs "<<b.getParameters()[i]->getValue()<<"\n";require(std::abs(a.getParameters()[i]->getValue()-b.getParameters()[i]->getValue())<0.0001f,"Recall mismatch");}
 b.setStateInformation("bad",3);
 for(int i=0;i<15;++i)require(std::abs(a.getParameters()[i]->getValue()-b.getParameters()[i]->getValue())<0.0001f,"Malformed state changed values");
 for(const auto& s:uveq::specs){auto* p=a.state.getParameter(s.id);require(std::abs(p->convertFrom0to1(0)-s.min)<0.001f,"Minimum");require(std::abs(p->convertFrom0to1(1)-s.max)<0.001f,"Maximum");}
 for(int channels:{1,2,8,64}){
  auto set=juce::AudioChannelSet::discreteChannels(channels);if(channels==1)set=juce::AudioChannelSet::mono();if(channels==2)set=juce::AudioChannelSet::stereo();
  juce::AudioProcessor::BusesLayout l;l.inputBuses.add(set);l.outputBuses.add(set);require(a.setBusesLayout(l),"Matching layout rejected");
  a.prepareToPlay(48000,1024);
  for(float on : {0.f,1.f}) {a.state.getParameter("in")->setValueNotifyingHost(on); for(int n:{0,1,64,1024}){audioTest<float>(a,channels,n);audioTest<double>(a,channels,n);}}a.releaseResources();
 }
 juce::AudioProcessor::BusesLayout bad;bad.inputBuses.add(juce::AudioChannelSet::mono());bad.outputBuses.add(juce::AudioChannelSet::stereo());require(!a.isBusesLayoutSupported(bad),"Mismatched layout accepted");
 {
  std::unique_ptr<juce::AudioProcessorEditor> editor(b.createEditor());
  juce::Component* controls=nullptr;
  for(auto* candidate:editor->getChildren()) if(candidate->getNumChildComponents()==15) controls=candidate;
  require(controls!=nullptr,"Control panel missing");int knobCount=0,buttonCount=0;
  for(auto* component:controls->getChildren()) {
   if(auto* knob=dynamic_cast<juce::Slider*>(component)){const auto& spec=uveq::specs[(size_t)knobCount++];knob->setValue(spec.max,juce::sendNotificationSync);require(std::abs(b.state.getRawParameterValue(spec.id)->load()-spec.max)<0.001f,"GUI to parameter");b.state.getParameter(spec.id)->setValueNotifyingHost(0);require(std::abs(knob->getValue()-spec.min)<0.001,"Parameter to GUI");}
   if(auto* button=dynamic_cast<juce::ToggleButton*>(component)){const auto* id=uveq::toggleIDs[(size_t)buttonCount++];button->setToggleState(false,juce::sendNotificationSync);require(b.state.getRawParameterValue(id)->load()==0,"Toggle off");button->setToggleState(true,juce::sendNotificationSync);require(b.state.getRawParameterValue(id)->load()==1,"Toggle on");}
  }
  require(knobCount==12&&buttonCount==3,"GUI control count");
 }
 if(argc>1){RecallProcessor defaults;std::unique_ptr<juce::AudioProcessorEditor> e(defaults.createEditor());e->setSize(578,1024);auto img=e->createComponentSnapshot(e->getLocalBounds());juce::FileOutputStream out{juce::File(argv[1])};require(out.openedOk(),"Preview output");juce::PNGImageFormat png;require(png.writeImageToStream(img,out),"Preview encoding");}
 std::cout<<"PASS: 15 parameters; ranges; state round-trip; corrupt state; matching buses; float/double bit-exact passthrough, 1/2/8/64 channels, 0/1/64/1024 samples.\n";
 return 0;
 }catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}
}
