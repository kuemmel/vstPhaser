#ifndef __plugin__
#define __plugin__
#include "pluginadapter.h"
#include "PluginProcessor.h"
namespace Steinberg {
namespace Vst {

// change these numbers!
// every plugin needs a unique UID
static const FUID PluginUID (0x89E8AE6F, 0x92554F63, 0x96FAE414, 0x3C945A18);

// define parameter tags here:
enum
{
	/** parameter ID */
	kGainId = 0
};


class Phaser : public PluginAdapter
{
public:
	Phaser ();
	virtual ~Phaser (){}
	tresult PLUGIN_API initialize (FUnknown* context);
	tresult PLUGIN_API process (ProcessData& data);
	void startProcessing(int numChannels, SampleRate sampleRate);	
	static FUnknown* createInstance (void*) { return (IAudioProcessor*)new Phaser (); }
private:
	int numChannels;
	SampleRate sampleRate;
private:
	PluginProcessor leftProcessor;
	PluginProcessor rightProcessor;
};




}} // namespaces

#endif //__plugin__
