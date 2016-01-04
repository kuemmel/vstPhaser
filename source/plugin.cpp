#include "plugin.h"
#include <math.h>
#include <stdio.h>

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
/**
 * Initialize the plugin, add parameters
 * mix
 * resonance
 * speed
 * stages
 * depth
 * state - on/off
 **/
tresult PLUGIN_API Phaser::initialize (FUnknown* context){
	//how much of the sound is coming from the effect
	parameters.addParameter(new RangeParameter(STR16("Mix"), kGainId, STR16("%"), 0, 100, 0));
	//The amount of reverb that is mixed on the notch-filter
	parameters.addParameter(new RangeParameter(STR16("Resonance"), kGainId, STR16("%"), 0, 100, 0));
	//the speed at which the notchfilter oscillates("speed" is traditionally used)
	parameters.addParameter(new RangeParameter(STR16("Speed"), kGainId, STR16("Hz"), -100, 6, 0));
	//amount of notchfilters
	parameters.addParameter(new RangeParameter(STR16("Stages"), kGainId, STR16("Stages"), 0, 3, 0));
	//effect of the notchfilters on the basis
	parameters.addParameter(new RangeParameter(STR16("Depth"), kGainId, STR16("dB"), -100, 6, 0));
	//on/of @TODO : add something else than a RangeParameter here?
	parameters.addParameter(new RangeParameter(STR16("state"), kGainId, STR16("on/off"), 0,1, 0));

	// fix for RangeParameter (default value is not yet set)
	for(int i = 0; i < parameters.getParameterCount(); i++){
		Parameter* p = parameters.getParameterByIndex(i);
		p->setNormalized(p->getInfo().defaultNormalizedValue);
	}
	return PluginAdapter::initialize(context);	
}

//-----------------------------------------------------------------------------
Phaser::Phaser ()
: numChannels(0)
, sampleRate(0)
{
}
//-----------------------------------------------------------------------------
void Phaser::startProcessing(int numChannels, SampleRate sampleRate){
	this->numChannels = numChannels;
	this->sampleRate = sampleRate;
	leftProcessor.initialize(sampleRate);
	rightProcessor.initialize(sampleRate);
}
tresult PLUGIN_API Phaser::process (ProcessData& data)
{
    if (hasInputParameterChanged(data, kGainId)){
        float paramValue = getInputParameterChange(data, kGainId);
		float dB = 106 * paramValue - 100;
		float gain = pow(10, dB/20);
    }
 	if (numChannels > 0){
		float* leftInputChannel = data.inputs[0].channelBuffers32[0];
		float* leftOutputChannel = data.outputs[0].channelBuffers32[0];
		leftProcessor.process(leftInputChannel, leftOutputChannel, data.numSamples);
	}
	if (numChannels > 1){
		float* rightInputChannel = data.inputs[0].channelBuffers32[1];
		float* rightOutputChannel = data.outputs[0].channelBuffers32[1];
		rightProcessor.process(rightInputChannel, rightOutputChannel, data.numSamples);
	}
	return kResultTrue;
}


}} // namespaces

