#include "AllpassFilter.h"
#define _USE_MATH_DEFINES // use pi constant
#include <cmath>


AllpassFilter::AllpassFilter(float sample_rate, float new_gain, float frequency_step, float minimum_frequency, float Q) {
	this->sampleRate;
	float dampening = std::sqrt(1 - (new_gain*new_gain)) / new_gain;
	this->coefficients = new float*[max_coefficients];

	for (int i = 0; i < max_coefficients; i++) {
		this->coefficients[i] = new float[5];
		
		float wo = 2 * M_PI * (frequency_step*i + minimum_frequency) / sample_rate;
		this->coefficients[i][0] = 1 / (1 + dampening * std::tan(wo / (Q * 2)));
		this->coefficients[i][1] = std::cos(wo);
		this->coefficients[i][2] = this->coefficients[i][0];
		this->coefficients[i][3] = 2*this->coefficients[i][0] * this->coefficients[i][1];
		this->coefficients[i][4] = 2*this->coefficients[i][0] - 1;
	}
}

AllpassFilter::~AllpassFilter(){
	delete[] coefficients;
}

void AllpassFilter::init(int notch_frequency) {
	this->H.a = this->coefficients[notch_frequency][0];
	this->H.b = this->coefficients[notch_frequency][1];
	this->H.d[0] = this->coefficients[notch_frequency][2];
	this->H.d[1] = this->coefficients[notch_frequency][3];
	this->H.d[2] = this->coefficients[notch_frequency][4];
}

struct NotchFilter AllpassFilter::getFilterObject() {
	return this->H;
}


float AllpassFilter::process(float input) {
	//delay inputs
	this->H.x[0] = this->H.x[1];
	this->H.x[1] = this->H.x[2];
	this->H.x[2] = input;

	//reuse outputs
	this->H.y[0] = this->H.y[1];
	this->H.y[1] = this->H.y[2];

	//calc new result
	this->H.y[2] = this->H.d[0] * this->H.x[2] - this->H.d[1] * this->H.x[1] + this->H.d[0] * this->H.x[0] + this->H.d[1] * this->H.y[1] - this->H.d[2] * this->H.y[0];
	
	return H.y[2];

}

