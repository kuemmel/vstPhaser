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
	parameters.addParameter(new RangeParameter(STR16("Mix"), kMixId, STR16("dB"), -100, 6, 6));
	//The amount of reverb that is mixed on the notch-filter
	parameters.addParameter(new RangeParameter(STR16("Resonance"), kResonanceId, STR16("%"), 0, 100, 0));
	//the speed at which the notchfilter oscillates("speed" is traditionally used)
	parameters.addParameter(new RangeParameter(STR16("Speed"), kSpeedId, STR16("Hz"), 0, 100, 0));
	//amount of notchfilters
	parameters.addParameter(new RangeParameter(STR16("Stages"), kStagesId, STR16("Stages"), 0, 3, 0));
	//effect of the notchfilters on the basis
	parameters.addParameter(new RangeParameter(STR16("Depth"), kDepthId, STR16("dB"), -100, 6, -100));
	//on/of @TODO : add something else than a RangeParameter here?
	parameters.addParameter(new RangeParameter(STR16("state"), kStateId, STR16("on/off"), 0, 1, 1));

	// fix for RangeParameter (default value is not yet set)
	for(int i = 0; i < parameters.getParameterCount(); i++){
		Parameter* p = parameters.getParameterByIndex(i);
		p->setNormalized(p->getInfo().defaultNormalizedValue);
	}
	return PluginAdapter::initialize(context);	

	// set (placeholder) variables
	mix = 100;
	resonance = 0;
	speed = 0;
	stages = 0;
	depth = -100;
	state = 1;
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
    if (hasInputParameterChanged(data, kMixId)) {
        float paramValue = getInputParameterChange(data, kMixId);
		float dB = 106 * paramValue - 100;
		float gain = pow(10, dB/20);
		mix = gain;
		leftProcessor.setGain(gain);
		rightProcessor.setGain(gain);
    }
    if(hasInputParameterChanged(data, kResonanceId)) {
        float paramValue = getInputParameterChange(data,kResonanceId);
		
        
    }
	if(hasInputParameterChanged(data, kSpeedId)) {
        float paramValue = getInputParameterChange(data,kSpeedId);
        
    }
	if(hasInputParameterChanged(data, kStagesId)) {
        float paramValue = getInputParameterChange(data,kStagesId);
        
    }
	if(hasInputParameterChanged(data, kDepthId)) {
        float paramValue = getInputParameterChange(data,kDepthId);
        
    }
	if(hasInputParameterChanged(data, kStateId)) {
        float paramValue = getInputParameterChange(data,kStateId);
        
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

