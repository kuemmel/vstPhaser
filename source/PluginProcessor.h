#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H

#include "SecondOrderFilter.h"
/**
 * Phaser plugin processor
 * A phaser is an audio effect, for a description see here: https://en.wikipedia.org/wiki/Phaser_(effect)
 * this effect is made for a vst host to be used as a realtime audio effect on an instrument like a guitar.
 *
 * uses a NotchFilter and a Oscillator for the effect
 *
 * @sampleRate: sampleRate of the audio stream
 * @mix the amount of the processed part of the end result
 * @depth strength of the notchfilter
 * @resonance: amount; further emphasise the sound by adding a feedback from a previous old (processed) signal
 * @oscillatorFrequency: The frequency of the effect (set by setSpeed)
 * @stages: How often the signal is processed by the notch-filter
 **/
class PluginProcessor{
public:
	PluginProcessor();
	~PluginProcessor();
	void initialize(float sampleRate, float mix, float resonance, float speed, float depth, unsigned short stages);
	float processOneSample(float);
	void process(float* input, float*output, int numberOfSamples);

	//setter methods
	void setMix(float mix);
	void setGain(float gain);
	void setResonance(float resonance);
	void setSpeed(float speed);
	void setDepth(float depth);
	void setStages(unsigned short stages);
private:
	double getTargetFrequency();

private:
	float sampleRate;
	unsigned long oscillatorIndex;
	float oscillatorFrequency;

	double minFrequency;
	double maxFrequency;

	float depth;
	float mix;
	unsigned short stages;
	int resonance;

	double prevBandpass;

	double m_fq;

	double frequencyChange;

	double* shiftedOutputs;

	SecondOrderFilter* sof;
	SecondOrderFilter* sof2;

};
#endif