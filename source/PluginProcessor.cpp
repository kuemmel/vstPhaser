#include "PluginProcessor.h"
#include "AllpassFilter.h"

#define _USE_MATH_DEFINES
#include <math.h>

PluginProcessor::PluginProcessor(){

	oscillatorIndex = 0;
	oscillatorFrequency = 100;

	minFrequency = 500;
	maxFrequency = 18000;

	depth = 1;
	mix = 0.5;
	stages = 1;
	resonance = 0;

	m_fq = 0.49;

	prevBandpass = 0;

	shiftedOutputs = new double[resonance];
	for (int i = 0; i < resonance; i++){
		shiftedOutputs[i] = 0;
	}

	sof = new SecondOrderFilter();
	sof2 = new SecondOrderFilter();
	frequencyChange = 0.25;


}

PluginProcessor::~PluginProcessor(){

	delete sof;
	delete sof2;

	delete shiftedOutputs;

	delete allpass;
}

void PluginProcessor::initialize(float sampleRate, float mix, float resonance, float speed, float depth, unsigned short stages){
	this->sampleRate = sampleRate;
	this->mix = mix;
	setResonance(resonance);
	this-> oscillatorFrequency = speed;
    this->depth = depth;
    this->stages = stages;
	sof->initialize(sampleRate);
	sof2->initialize(sampleRate);

	this->allpass = new AllpassFilter(sampleRate, 0.01, 50, 100, m_fq*10);
	this->allpass->init(maxFrequency-500);
}

double PluginProcessor::getTargetFrequency(){
	double value = sin(2 * M_PI * oscillatorIndex * oscillatorFrequency / sampleRate);
	oscillatorIndex++;
	return minFrequency + (maxFrequency - minFrequency) * (value*0.5 + 0.5);
}

float PluginProcessor::processOneSample(float input){

	float output = this->allpass->process(input);

	return output;
}

void PluginProcessor::setMix(float mix) {
    this->mix = mix;
}
void PluginProcessor::setResonance(float resonance) {
    this->resonance = resonance;
	delete shiftedOutputs;
	shiftedOutputs = new double[this->resonance];
	for (int i = 0; i < this->resonance; i++){
		shiftedOutputs[i] = 0;
	}
}
void PluginProcessor::setSpeed(float speed){
    this->oscillatorFrequency = speed;
}
void PluginProcessor::setDepth(float depth) {
    this->depth = depth;
}
void PluginProcessor::setStages(unsigned short stages) {
    this->stages = stages;
}


void PluginProcessor::process(float* input, float*output, int numberOfSamples){
	for(int i = 0; i < numberOfSamples; i++){
		output[i] = processOneSample(input[i]);
	}
}