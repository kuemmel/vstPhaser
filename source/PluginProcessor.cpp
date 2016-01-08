#define _USE_MATH_DEFINES
#include <math.h>

#include "PluginProcessor.h"
#include "stages.h"


PluginProcessor::PluginProcessor(){
	this->oscillatorIndex = 0;
	this->oscillatorFrequency = 100;

	this->minFrequency = 500;
	this->maxFrequency = 15000;

	this->stages = new Stages(0);
	this->depth = 1;
	this->mix = 0.5;
	this->resonance = 0;
	this->resonanceBufferReadPos = 0;

	this->m_fq = 0.49;
	this->prevBandpass = 0;

	this->sof = new SecondOrderFilter();
	this->frequencyChange = 0.25;
}

PluginProcessor::~PluginProcessor(){
	delete this->stages;
	delete this->sof;
	delete this->resonanceBuffer;
}

void PluginProcessor::initialize(float sampleRate, float mix, float resonance, float speed, float depth, unsigned short stages){
	this->sampleRate = sampleRate;
	this->mix = mix;
	this->setResonance(resonance);
	this-> oscillatorFrequency = speed;
    this->depth = depth;
	this->sof->initialize(sampleRate);

	this->resonanceBufferLength = static_cast<int>(sampleRate);
	this->resonanceBuffer = new double[resonanceBufferLength];
	for (int i = 0; i < resonanceBufferLength; i++)
		this->resonanceBuffer[i] = 0;
}

double PluginProcessor::getTargetFrequency(){
	double value = sin(2 * M_PI * this->oscillatorIndex * this->oscillatorFrequency / this->sampleRate);
	this->oscillatorIndex++;
	return this->minFrequency + (this->maxFrequency - this->minFrequency) * (value*0.5 + 0.5);
}

float PluginProcessor::processOneSample(float input) {

	float output = input;
	double filterQ = m_fq*depth;

	sof->set(ALLPASS, this->getTargetFrequency(), filterQ, -1000);
	this->stages->reset(); // just to be sure, should not be needed
	while(this->stages->checkIndex()) {
		output = sof->processOneSample(output);
	}

	output = input*(1-mix) + output*mix;

	if (this->resonance > 0){
		this->resonanceBuffer[(this->resonanceBufferReadPos + this->resonance) % this->resonanceBufferLength] = output;
		double outputMix = this->resonanceBuffer[this->resonanceBufferReadPos++];
		this->resonanceBufferReadPos %= this->resonanceBufferLength;

		return output*0.5 + outputMix*0.5;
	}
	else return output;
}

/*for (this->stages->reset(); this->stages->checkIndex();){
output = sof->processOneSample(output);
}

for (int i = 0; i<12; i++) {
output = sof->processOneSample(output);
}*/

void PluginProcessor::setMix(float mix) {
    this->mix = mix;
}
void PluginProcessor::setResonance(float resonance) {
	this->resonance = static_cast<int>(floor(resonance));
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
		output[i] = this->processOneSample(input[i]);
	}
}