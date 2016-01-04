#include "PluginProcessor.h"

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

	shiftedOutputs = new double[depth];
	for (int i = 0; i < resonance; i++){
		shiftedOutputs[0];
	}

	sof = new SecondOrderFilter();
	sof2 = new SecondOrderFilter();
	frequencyChange = 0.25;
}

PluginProcessor::~PluginProcessor(){

	delete sof;
	delete sof2;

	delete shiftedOutputs;
}

void PluginProcessor::initialize(float sampleRate){
	this->sampleRate = sampleRate;
	sof->initialize(sampleRate);
}

/**
	FUCKING TO DO::
	Oscillator fixen -.-
*/
double PluginProcessor::getTargetFrequency(){
	/*double value = sin(2 * M_PI * oscillatorIndex * oscillatorFrequency / sampleRate);
	oscillatorIndex++;
	return minFrequency + (maxFrequency - minFrequency) * (value*0.5 + 0.5);*/
	minFrequency += frequencyChange;
	if (minFrequency >= maxFrequency){
		minFrequency = maxFrequency;
		frequencyChange *= -1;
	}
	else if (minFrequency <= 100){
		minFrequency = 100;
		frequencyChange *= -1;
	}
	return minFrequency;
}

float PluginProcessor::processOneSample(float input){

	float output = input;
	float saveOut = input;

	double filterQ = m_fq / 2 + m_fq*depth / 100;

	double frequency = getTargetFrequency();
	sof->set(NOTCH, frequency, filterQ, -1000);

	for (unsigned int i = 0; i < stages*2; i++){

		output = sof->processOneSample(output);
	}
	
	sof2->set(BANDPASS, frequency, filterQ, -1000);
	for (unsigned int i = 0; i < stages * 2; i++){

		saveOut = sof2->processOneSample(saveOut);
	}

	output = output*(1 - mix) + prevBandpass*(mix);
	prevBandpass = saveOut;

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

void PluginProcessor::process(float* input, float*output, int numberOfSamples){
	for(int i = 0; i < numberOfSamples; i++){
		output[i] = processOneSample(input[i]);
	}
}