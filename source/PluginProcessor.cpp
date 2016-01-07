#define _USE_MATH_DEFINES
#include <math.h>

#include "PluginProcessor.h"
#include "stages.h"


PluginProcessor::PluginProcessor(){

	oscillatorIndex = 0;
	oscillatorFrequency = 100;

	minFrequency = 500;
	maxFrequency = 18000;

	depth = 1;
	mix = 0.5;
	this->stages = new Stages(0);
	resonance = 0;

	m_fq = 0.49;

	prevBandpass = 0;

	shiftedOutputs = new double[resonance];
	for (int i = 0; i < resonance; i++){
		shiftedOutputs[i] = 0;
	}

	sof = new SecondOrderFilter();
	frequencyChange = 0.25;
}

PluginProcessor::~PluginProcessor(){
	delete sof;
	delete shiftedOutputs;
}

void PluginProcessor::initialize(float sampleRate, float mix, float resonance, float speed, float depth, unsigned short stages){
	this->sampleRate = sampleRate;
	this->mix = mix;
	setResonance(resonance);
	this-> oscillatorFrequency = speed;
    this->depth = depth;
	sof->initialize(sampleRate);
}

double PluginProcessor::getTargetFrequency(){
	double value = sin(2 * M_PI * oscillatorIndex * oscillatorFrequency / sampleRate);
	oscillatorIndex++;
	return minFrequency + (maxFrequency - minFrequency) * (value*0.5 + 0.5);
	/*minFrequency += frequencyChange;
	if (minFrequency >= maxFrequency){
		minFrequency = maxFrequency;
		frequencyChange *= -1;
	}
	else if (minFrequency <= 100){
		minFrequency = 100;
		frequencyChange *= -1;
	}
	return minFrequency;*/
}

float PluginProcessor::processOneSample(float input){

	float output = input;
	double filterQ = m_fq*depth;
	double frequency = getTargetFrequency();
	sof->set(ALLPASS, frequency, filterQ, -1000);

	for (int i = 0; i<12; i++) {
		output = sof->processOneSample(output);
	}

	output = input*(1 - mix) + (output/*0.5+prevBandpass*0.5*/)*(mix);

	if (resonance > 0){
		double outputMix = shiftedOutputs[resonance - 1];
		for (int i = resonance - 1; i > 0; i--){
			shiftedOutputs[i] = shiftedOutputs[i - 1];
		}

		shiftedOutputs[0] = output;

		return output*0.5 + outputMix*0.5;
	}
	else
		return output;
}

/*for (this->stages->reset(); this->stages->checkIndex();){
output = sof->processOneSample(output);
}*/

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
void PluginProcessor::setStages(unsigned short stage) {
	this->stages->setStage(stage);
}


void PluginProcessor::process(float* input, float*output, int numberOfSamples){
	for(int i = 0; i < numberOfSamples; i++){
		output[i] = processOneSample(input[i]);
	}
}