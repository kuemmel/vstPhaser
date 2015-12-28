#ifndef __SECONDORDERFILTER_H
#define __SECONDORDERFILTER_H
#include <string>


const double MINFREQ = 10;
const double MAXQ = 24;
const double MAXGAIN = 24;
enum FilterType{
	PEAK,
	BANDPASS,
	NOTCH,
	LOWPASS,
	HIGHPASS,
	LOW_SHELF,
	HIGH_SHELF,
	NUMTYPES
};

class SecondOrderFilter{
public:

	SecondOrderFilter();
	void initialize(double sampleRate);
	void set(FilterType type, double frequency, double Q, double gain);
	void set(double a0, double a1, double a2, double b0, double b1, double b2);
	float processOneSample(float input);
private:
	double sampleRate;
	double a, b, c, d, e;
	double x1, x2, y1, y2;
};
#endif