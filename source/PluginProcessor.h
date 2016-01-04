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
	void initialize(float sampleRate, unsigned short mix, unsigned short resonance, unsigned short speed, unsigned short depth, unsigned short stages);
	float processOneSample(float);
	void process(float* input, float*output, int numberOfSamples);

	//setter methods
	void setMix(unsigned short mix);
	void setGain(unsigned short gain);
	void setResonance(unsigned short resonance);
	void setSpeed(unsigned short speed);
	void setDepth(unsigned short depth);
	void setStages(unsigned short stages);
private:
	double getTargetFrequency();

private:
	float sampleRate;
	unsigned long oscillatorIndex;
	unsigned int oscillatorFrequency;

	double minFrequency;
	double maxFrequency;

	unsigned int depth;
	float gain;
	
	unsigned short mix;
	unsigned short resonance;
	unsigned short stages;

	double m_fq;

	double frequencyChange;

	double* x1s;
	double* x2s;
	double* y1s;
	double* y2s;

	double* shiftedOutputs;

	SecondOrderFilter* sof;
	SecondOrderFilter* sof2;

};
#endif