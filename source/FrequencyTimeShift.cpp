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

void FrequencyTimeShift::add(double input, double frequency, double phaseShift){
	while (phaseShift >= 2*M_PI)
		phaseShift -= 2 * M_PI;
	
	int positionToWrite = (sampleRate / frequency) * (phaseShift / (2 * M_PI));
	positionToWrite += bufferReadPosition;
	positionToWrite %= bufferLength;

	if (buffer[positionToWrite] != 0)
		buffer[positionToWrite] = buffer[positionToWrite] * (1 - mixByExistingValue) + input*mixByExistingValue;
	else
		buffer[positionToWrite] = input;
}

double FrequencyTimeShift::get(){
	double value = buffer[bufferReadPosition];
	buffer[bufferReadPosition++] = 0;
	bufferReadPosition %= bufferLength;
	return value;
}