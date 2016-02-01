#define _USE_MATH_DEFINES
#include <math.h>
#include "allpass.h"

AllpassFilter::AllpassFilter() : sampleRate(44100),a(0), b(0), c(0), d(0), e(0), x1(0), x2(0), y1(0), y2(0){

}

AllpassFilter::~AllpassFilter() {

}

AllpassFilter::AllpassFilter(double sampleRate, double frequency, double Q, double gain) {
	this->setSampleRate(sampleRate);
	set(frequency, Q, gain);
}

void AllpassFilter::setSampleRate(double sampleRate){
	this->sampleRate = sampleRate;
}

void AllpassFilter::set(double frequency, double Q, double gain){
	// gain = gain in dB
	// frequency = freq of interest
	// Q = Q
	// m_ type is the filter type
	if (frequency < this->MINIMUM_FREQUENCY) {
		frequency = this->MINIMUM_FREQUENCY;
	}
	double amp = pow(10.0, gain / 40.0);
	double w = 2.0 * M_PI * (frequency / sampleRate);
	double sinw = sin(w);
	double cosw = cos(w);
	double alpha = sinw / (2.0 * Q);
	double beta = sqrt(amp) / Q;
	double fkap = (float)exp(-w);

	double a0, a1, a2, b0, b1, b2;

	b0 = 1 - alpha;
	b1 = -2.0 * cosw;
	b2 = 1 + alpha;
	a0 = 1.0 + alpha;
	a1 = 2.0 * cosw;
	a2 = alpha - 1.0;                                         
	set(a0, a1, a2, b0, b1, b2);
}

void AllpassFilter::set(double a0, double a1, double a2, double b0, double b1, double b2){
	a = b0 / a0;
	b = b1 / a0;
	c = b2 / a0;
	d = a1 / a0;
	e = a2 / a0;
}

float AllpassFilter::processOneSample(float input){
	double output = (a*input) + (b*x1) + (c*x2) + (d*y1) + (e*y2);
	x2 = x1;
	x1 = input;
	y2 = y1;
	y1 = output;
	return (float)output;
}