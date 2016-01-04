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
	//how much of the sound is coming from the filter(s)
	parameters.addParameter(new RangeParameter(STR16("Mix"), kMixId, STR16("%"), 0, 100, 50));
	//The amount of reverb that is mixed on the notch-filter
	parameters.addParameter(new RangeParameter(STR16("Resonance"), kResonanceId, STR16("%"), 0, 100, 0));
	//the speed at which the notchfilter oscillates("speed" is traditionally used)
	parameters.addParameter(new RangeParameter(STR16("Speed"), kSpeedId, STR16("Hz"), 0, 100, 50));
	//amount of notchfilters the signal is going through, 0 is off
	parameters.addParameter(new RangeParameter(STR16("Stages"), kStagesId, STR16("Stages"), 0, 4, 0));
	//effect of the notchfilters on the basis
	parameters.addParameter(new RangeParameter(STR16("Depth"), kDepthId, STR16("dB"), 0, 100, 50));

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
	leftProcessor.initialize(sampleRate,50,0,50,50,0);
	rightProcessor.initialize(sampleRate, 50, 0, 50, 50, 0);
}
tresult PLUGIN_API Phaser::process (ProcessData& data)
{
    if (hasInputParameterChanged(data, kMixId)) {
        float paramValue = getInputParameterChange(data, kMixId);
		float dB = 106 * paramValue - 100;
		float gain = pow(10, dB/20);
		leftProcessor.setMix(gain);
		rightProcessor.setMix(gain);
    }
    if(hasInputParameterChanged(data, kResonanceId)) {
        float paramValue = getInputParameterChange(data,kResonanceId);
		//hack
		setOutputParameterChange(data, kResonanceId, floor(paramValue));
    }
	if(hasInputParameterChanged(data, kSpeedId)) {
        float paramValue = getInputParameterChange(data,kSpeedId);
		//hack
		setOutputParameterChange(data, kSpeedId, floor(paramValue));
    }
	if(hasInputParameterChanged(data, kStagesId)) {
        float paramValue = getInputParameterChange(data,kStagesId);
		//hack
		setOutputParameterChange(data, kStagesId, floor(paramValue));
    }
	if(hasInputParameterChanged(data, kDepthId)) {
        float paramValue = getInputParameterChange(data,kDepthId);
		//hack
		setOutputParameterChange(data, kDepthId, floor(paramValue));
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

