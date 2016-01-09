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
tresult PLUGIN_API Phaser::initialize(FUnknown* context){
	//how much of the sound is coming from the filter(s)
	
	Parameter* parameter = new RangeParameter(STR16("Mix"), kMixId, STR16("%"), 0, 100, 50);
	parameter->setPrecision(0);
	Parameter* parameter1 = new RangeParameter(STR16("Resonance"), kResonanceId, STR16("%"), 0, 1000, 0);
	parameter1->setPrecision(0);
	Parameter* parameter2 = new RangeParameter(STR16("Speed"), kSpeedId, STR16("Hz"), 0, 2, 0.5);
	parameter2->setPrecision(2);
	Parameter* parameter3 = new RangeParameter(STR16("Stages"), kStagesId, STR16("Stages"), 0, 4, 0);
	parameter3->setPrecision(0);
	Parameter* parameter4 = new RangeParameter(STR16("Depth"), kDepthId, STR16("dB"), 0.001, 2, 0.49);
	parameter4->setPrecision(2);
	
    parameters.addParameter(parameter); 
    parameters.addParameter(parameter1); 
    parameters.addParameter(parameter2); 
    parameters.addParameter(parameter3);
	parameters.addParameter(parameter4);

	// fix for RangeParameter (default value is not yet set)
	/*for(int i = 0; i < parameters.getParameterCount(); i++){
		Parameter* p = parameters.getParameterByIndex(i);
		p->setNormalized(p->getInfo().defaultNormalizedValue);
	}*/
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
	leftProcessor.initialize(sampleRate,0.5,0,0.5,0.49,0); 
	rightProcessor.initialize(sampleRate, 0.5, 0, 0.5, 0.49, 0);
}
tresult PLUGIN_API Phaser::process (ProcessData& data)
{
    if (hasInputParameterChanged(data, kMixId)) {
        float paramValue = getInputParameterChange(data, kMixId);
		//float dB = 106 * paramValue - 100;
		//float gain = pow(10, dB/20);
		//float test = getPlainValue(kMixId);
		leftProcessor.setMix(paramValue);
		rightProcessor.setMix(paramValue);
    }
    if(hasInputParameterChanged(data, kResonanceId)) {
        //float paramValue = getInputParameterChange(data,kResonanceId); 
		float paramValue = getPlainValue(kResonanceId);

		leftProcessor.setResonance(paramValue);
		rightProcessor.setResonance(paramValue);
    }
	if(hasInputParameterChanged(data, kSpeedId)) {
		//float paramValue = getInputParameterChange(data, kSpeedId);
		float paramValue = getPlainValue(kSpeedId);
		leftProcessor.setSpeed(paramValue);
		rightProcessor.setSpeed(paramValue);
    }
	if(hasInputParameterChanged(data, kStagesId)) {
		//float paramValue = getInputParameterChange(data, kStagesId);
		float paramValue = getPlainValue(kStagesId);
 		leftProcessor.setStages(paramValue);
		rightProcessor.setStages(paramValue);
    }
	if(hasInputParameterChanged(data, kDepthId)) {
		//float paramValue = getInputParameterChange(data, kDepthId);
		float paramValue = getPlainValue(kDepthId);
		leftProcessor.setDepth(paramValue);
		rightProcessor.setDepth(paramValue);
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

