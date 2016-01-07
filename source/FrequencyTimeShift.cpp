#include "FrequencyTimeShift.h"

FrequencyTimeShift::FrequencyTimeShift(double mixByExistingValue){
	this->mixByExistingValue = mixByExistingValue;
	buffer = nullptr;
	bufferLength = 0;
	bufferReadPosition = 0;
}

FrequencyTimeShift::~FrequencyTimeShift(){
	delete[] buffer;
}

void FrequencyTimeShift::initialize(double sampleRate){
	this->sampleRate = sampleRate;
	delete[] buffer;
	bufferLength = (int)sampleRate * 2;
	buffer = new double[bufferLength];
	for (int i = 0; i < bufferLength; i++)
		buffer[i] = 0;
}

void FrequencyTimeShift::add(double input, double frequency, int width, double phaseShift){
	while (phaseShift >= 2*M_PI)
		phaseShift -= 2 * M_PI;
	
	int positionToWrite = (sampleRate / frequency) * (phaseShift / (2 * M_PI));
	positionToWrite += bufferReadPosition;
	positionToWrite %= bufferLength;

	for (int i = positionToWrite - width; i <= positionToWrite + width; i++){
		int ii = i % bufferLength;
		if (ii < 0)
			ii += bufferLength;

		double valueToWrite = input*(1 - (abs(i - positionToWrite) / ((width == 0) ? 1 : width)));

		if (buffer[ii] != 0)
			buffer[ii] = buffer[ii] * (1 - mixByExistingValue) + input*mixByExistingValue;
		else
			buffer[ii] = input;
	}
}

double FrequencyTimeShift::get(){
	double value = buffer[bufferReadPosition];
	buffer[bufferReadPosition++] = 0;
	bufferReadPosition %= bufferLength;
	return value;
}