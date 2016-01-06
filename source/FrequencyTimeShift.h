#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

class FrequencyTimeShift{
public:
	FrequencyTimeShift(double mixByExistingValue = 0.5);
	~FrequencyTimeShift();

	void initialize(double sampleRate);

	void add(double input, double frequency, double phaseShift = M_PI);
	double get();

private:
	double sampleRate;
	double mixByExistingValue;
	double* buffer;
	int bufferLength;
	int bufferReadPosition;
};