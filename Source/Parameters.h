#pragma once
#include <juce_audio_utils/juce_audio_utils.h>
#include <array>
namespace uveq {
struct Spec { const char* id; const char* name; float min,max,initial; const char* unit; int x,y; juce::uint32 colour; bool reverse; };
inline constexpr std::array<Spec,12> specs {{
 {"lfGain","LF Gain",-9,9,0," dB",182,474,0xff76558b,false},
 {"lfFreq","LF Frequency",0,3,1," Hz",406,275,0xff76558b,false},
 {"hfGain","HF Gain",-9,9,0," dB",989,474,0xffae4c7d,false},
 {"hfFreq","HF Frequency",0,3,1," kHz",765,275,0xffae4c7d,false},
 {"lmfGain","LMF Gain",-9,9,0," dB",182,1446,0xff5688a6,false},
 {"lmfFreq","LMF Frequency",0.1f,1.1f,0.6f," kHz",182,973,0xff5688a6,false},
 {"lmfQ","LMF Q",0.5f,3,1.75f,"",406,1210,0xff5688a6,true},
 {"hmfGain","HMF Gain",-9,9,0," dB",989,1446,0xff3c704c,false},
 {"hmfFreq","HMF Frequency",0.6f,9,3," kHz",989,973,0xff3c704c,false},
 {"hmfQ","HMF Q",0.5f,3,1.75f,"",765,1210,0xff3c704c,true},
 {"hpf","HPF",0,3,0," Hz",406,1768,0xffb3b8b1,false},
 {"trim","Trim",-12,12,0," dB",765,1768,0xff282c2d,false}
}};
inline constexpr std::array<const char*,3> toggleIDs {{"in","lmfFocus","hmfFocus"}};
inline bool discrete(int i) { return i==1 || i==3 || i==10; }
inline juce::StringArray choices(int i) {
 if(i==1) return {"30 Hz","50 Hz","60 Hz","90 Hz"};
 if(i==3) return {"8 kHz","12 kHz","16 kHz","20 kHz"};
 return {"OFF","30 Hz","40 Hz","50 Hz"};
}
// Piecewise scale read from the supplied HMF front-panel markings.
inline juce::NormalisableRange<float> hmfRange() {
 auto from=[](float,float,float t) { constexpr float v[]{0.6f,1,2,3,4,7,9}; constexpr float pos[]{0,.2f,.4f,.5f,.6f,.8f,1}; for(int i=1;i<7;++i) if(t<=pos[i]) return juce::jmap(t,pos[i-1],pos[i],v[i-1],v[i]); return 9.f; };
 auto to=[](float,float,float v) { constexpr float val[]{0.6f,1,2,3,4,7,9}; constexpr float pos[]{0,.2f,.4f,.5f,.6f,.8f,1}; for(int i=1;i<7;++i) if(v<=val[i]) return juce::jmap(v,val[i-1],val[i],pos[i-1],pos[i]); return 1.f; };
 return {0.6f,9.f,from,to};
}
}
