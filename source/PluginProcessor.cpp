#include "PluginProcessor.h"

PluginProcessor::PluginProcessor()
: gain(1)
{}

void PluginProcessor::initialize(float sampleRate){
	this->sampleRate = sampleRate;
}
void PluginProcessor::setGain(float gain){
	this->gain = gain;
}

float PluginProcessor::processOneSample(float input){
	return input * gain;
}
void PluginProcessor::process(float* input, float*output, int numberOfSamples){
	for(int i = 0; i < numberOfSamples; i++){
		output[i] = processOneSample(input[i]);
	}
}