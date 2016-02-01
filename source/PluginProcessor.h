#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H

#include "allpass.h"
#include "Stages.h"
#include "LowFrequencyOscillator.h"
/**
 * Phaser plugin processor
 * A phaser is an audio effect, for a description see here: https://en.wikipedia.org/wiki/Phaser_(effect)
 * this effect is made for a vst host to be used as a realtime audio effect on an instrument like a guitar.
 *
 * uses a NotchFilter and a Oscillator for the effect
 *
 * @sampleRate: sampleRate of the audio stream
 * @mix the amount of the processed part of the output stream
 * @depth strength of the notchfilter effect, e.i the width of the notch
 * @resonance: amount; further emphasise the sound by adding a feedback from a previous old (processed) signal
 * @oscillatorFrequency: The frequency of the effect (set by setSpeed)
 *		speed(and rate) are traditional terms to describe the control
 * @stages: How often the signal is processed by the notch-filter
 **/
class PluginProcessor{
public:
	/**
	 * Initialize the Plugin with some default values
	 **/
	PluginProcessor();
	~PluginProcessor();

	/**
	 * Override some of the defaults:
	 * @sampleRate: sampleRate of the audio stream
	 * @mix the amount of the processed part of the output stream
	 * @depth strength of the notchfilter effect, e.i the width of the notch
	 * @resonance: amount; further emphasise the sound by adding a feedback from a previous old (processed) signal
	 * @oscillatorFrequency: The frequency of the effect (set by setSpeed)
	 *		speed(and rate) are traditional terms to describe the control
	 * @stages: How often the signal is processed by the notch-filter
	 *
	 **/
	void initialize(float sampleRate, float mix, float resonance, float speed, float depth, unsigned short stages);

	/**
	 * Process a simple input value and return the result
	 **/
	float processOneSample(float);

	/**
	 * Process a number of samples return the return the pointer to a float array
	 **/
	void process(float* input, float* output, int numberOfSamples);

	//setter methods
	void setMix(float mix);
	void setGain(float gain);
	void setResonance(float resonance);
	void setSpeed(float speed);
	void setDepth(float depth);
	void setStages(unsigned short stages);
private:
	float sampleRate;
	float minFrequency;
	float maxFrequency;
	float depth;
	float mix;

	double prevBandpass;
	double m_fq;
	double frequencyChange;
	double* resonanceBuffer;
	int resonanceBufferLength;
	int resonanceBufferReadPos;
	int resonance;

	Stages* stages;
	AllpassFilter* allpass;
	LowFrequencyOscillator* lfo;

};
#endif