#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H

class PluginProcessor{
public:
	PluginProcessor();
	void initialize(float sampleRate);
	float processOneSample(float);
	void process(float* input, float*output, int numberOfSamples);
	void setGain(float gain);
private:
	float sampleRate;
	float gain;
};
#endif