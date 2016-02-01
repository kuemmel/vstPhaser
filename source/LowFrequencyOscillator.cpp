#define _USE_MATH_DEFINES
#include <math.h>

#include "LowFrequencyOscillator.h"

LowFrequencyOscillator::LowFrequencyOscillator() : sampleRate(44100), gain(1), frequency(100), x(0), index(0)
{

}

LowFrequencyOscillator::LowFrequencyOscillator(double sampleRate, double gain, double frequency) {
	this->sampleRate = sampleRate;
	this->gain = gain;
	this->frequency;
	this->x = 0;
	this->index = 0;
}

double LowFrequencyOscillator::process() {
	return this->get();
}

double LowFrequencyOscillator::get() {
	double value = sin(2 * M_PI * (this->frequency/this->sampleRate) * this->index);
	this->index++;
	return value;
}

void LowFrequencyOscillator::setSampleRate(double sampleRate) {
	this->sampleRate = sampleRate;
}

void LowFrequencyOscillator::setFrequency(double frequency) {
	this->frequency = frequency;
}

void LowFrequencyOscillator::setGain(double gain) {
	this->gain = gain;
}

double LowFrequencyOscillator::getSampleRate() {
	return this->sampleRate;
}

double LowFrequencyOscillator::getFrequency() {
	return this->frequency;
}

double LowFrequencyOscillator::getGain() {
	return this->gain;
}