#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H

#include "SecondOrderFilter.h"

class PluginProcessor{
public:
	PluginProcessor();
	~PluginProcessor();
	void initialize(float sampleRate);
	float processOneSample(float);
	void process(float* input, float*output, int numberOfSamples);

private:
	double getTargetFrequency();

private:
	float sampleRate;
	unsigned long oscillatorIndex;
	unsigned int oscillatorFrequency;

	double minFrequency;
	double maxFrequency;

	unsigned int depth;
	float mix;
	int stages;
	int resonance;

	double prevBandpass;

	double m_fq;

	double frequencyChange;

	double* shiftedOutputs;

	SecondOrderFilter* sof;
	SecondOrderFilter* sof2;

};
#endif