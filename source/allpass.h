#ifndef __ALLPASS_H
#define __ALLPASS_H
#include <string>


class AllpassFilter {
public:
	~AllpassFilter();
	AllpassFilter();
    /**
     *
     **/
	AllpassFilter(double sampleRate, double frequency, double Q, double gain);

	void initialize(double sampleRate);
	void set(double frequency, double Q, double gain);
	void set(double a0, double a1, double a2, double b0, double b1, double b2);
	void setSampleRate(double sampleRate);
	float processOneSample(float input);
private:
	double sampleRate;
	double a, b, c, d, e;
	double x1, x2, y1, y2;

	const double MINIMUM_FREQUENCY = 10;
	const double MAXIMUM_FREQUENCY = 24;
	const double MAXIMUM_GAIN = 24;
};
#endif