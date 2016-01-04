#include "PluginProcessor.h"

#define _USE_MATH_DEFINES
#include <math.h>

PluginProcessor::PluginProcessor(){
	oscillatorIndex = 0;
	oscillatorFrequency = 300;
	minFrequency = 500;
	maxFrequency = 18000;
	depth = 1;
	m_fq = 0.49;

	x1s = new double[depth];	x1s[0] = 0;
	x2s = new double[depth];	x2s[0] = 0;
	y1s = new double[depth];	y1s[0] = 0;
	y2s = new double[depth];	y2s[0] = 0;

	shiftedOutputs = new double[depth];
	for (int i = 0; i < depth; i++){
		shiftedOutputs[0];
	}

	sof = new SecondOrderFilter();
	sof2 = new SecondOrderFilter();
	frequencyChange = 0.25;
}

PluginProcessor::~PluginProcessor(){
	delete x1s;
	delete x2s;
	delete y1s;
	delete y2s;

	delete sof;

	delete shiftedOutputs;
}

void PluginProcessor::initialize(float sampleRate){
	this->sampleRate = sampleRate;
	sof->initialize(sampleRate);
}

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

	//float output = getTargetFrequency();
	float output = input;
	float saveOut = input;

	double frequency = getTargetFrequency();
	sof->set(NOTCH, frequency, m_fq, -1000);

	double outputMix = shiftedOutputs[depth - 1];
	for (int i = depth - 1; i > 0; i--){
		shiftedOutputs[i] = shiftedOutputs[i - 1];
	}

	for (unsigned int i = 0; i < depth; i++){

		/*sof->set(HIGHPASS, frequency + 10*depth, m_fq, -1000);
		double o1 = sof->processOneSample(output);

		sof->set(LOWPASS, frequency - 10*depth, m_fq, -1000);
		double o2 = sof->processOneSample(output);

		output = o1*0.5 + o2*0.5;*/

		//double frequency = getTargetFrequency();
		//sof->set(NOTCH, frequency, m_fq, -1000);
		output = sof->processOneSample(output);
	}
	
	/*sof2->set(BANDPASS, frequency, m_fq, -1000);
	for (unsigned int i = 0; i < depth; i++){

		saveOut = sof2->processOneSample(saveOut);
	}*/
	shiftedOutputs[0] = saveOut;

	/*for (unsigned int i = 1; i < depth; i++){

		double frequency = getTargetFrequency();
		double w = 2.0 * M_PI * (frequency / sampleRate);
		double sinw = sin(w);
		double cosw = cos(w);
		double alpha = sinw / (2.0 * m_fq);
		double amp = 0.001;

		double b0 = 1.0 + (alpha * amp);
		double b1 = -2.0 * cosw;
		double b2 = 1.0 - (alpha * amp);
		double a0 = 1.0 + (alpha / amp);
		double a1 = 2.0 * cosw;
		double a2 = -1.0 + (alpha / amp);

		double a = b0 / a0;
		double b = b1 / a0;
		double c = b2 / a0;
		double d = a1 / a0;
		double e = a2 / a0;

		output = (a*output) + (b*x1s[i - 1]) + (c*x2s[i - 1]) + (d*y1s[i - 1]) + (e*y2s[i - 1]);
		//output = (a*output) + (b*x1s[0]) + (c*x2s[0]) + (d*y1s[0]) + (e*y2s[0]);
		x2s[i] = x1s[i - 1];
		x1s[i] = input;
		y2s[i] = y1s[i-1];
		y1s[i] = output;
		/*x2s[0] = x1s[0];
		x1s[0] = input;
		y2s[0] = y1s[0];
		y1s[0] = output;*/
		
	//}

	return output*gain/*0.5 + outputMix*-0.5*/;
}

void PluginProcessor::setMix(float mix) {
	gain = mix;
}

void PluginProcessor::process(float* input, float*output, int numberOfSamples){
	for(int i = 0; i < numberOfSamples; i++){
		output[i] = processOneSample(input[i]);
	}
}