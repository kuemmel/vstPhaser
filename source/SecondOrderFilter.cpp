#include "SecondOrderFilter.h"

#define _USE_MATH_DEFINES
#include <math.h>


SecondOrderFilter::SecondOrderFilter()
: a(0), b(0), c(0), d(0), e(0), x1(0), x2(0), y1(0), y2(0)
{}

void SecondOrderFilter::initialize(double setSampleRate){
	sampleRate = setSampleRate;
}
void SecondOrderFilter::set(FilterType type, double m_freq, double m_fq, double m_fgain){
// m_fgain = gain in dB
// m_freq = freq of interest
// m_fq = Q
// m_ type is the filter type
	if (m_freq < MINFREQ) {
		m_freq = MINFREQ;
	}
	double amp = pow(10.0, m_fgain / 40.0);
	double w = 2.0 * M_PI * (m_freq / sampleRate);
	double sinw = sin(w);
	double cosw = cos(w);
	double alpha = sinw / (2.0 * m_fq);
	double beta = sqrt(amp) / m_fq;
	double fkap = (float) exp(-w);

	double a0, a1, a2, b0, b1, b2;

	if (type == PEAK){
		b0 = 1.0 + (alpha * amp);
		b1 = -2.0 * cosw;
		b2 = 1.0 - (alpha * amp);
		a0 = 1.0 + (alpha / amp);
		a1 = 2.0 * cosw;
		a2 = -1.0 + (alpha / amp);
	}
	else if (type == BANDPASS){
		b0 = alpha;
		b1 = 0.0;
		b2 = -alpha;
		a0 = 1.0 + alpha;
		a1 = 2.0 * cosw;
		a2 = alpha - 1.0;
	}
	else if (type == NOTCH){
		b0 = 1;
		b1 = -2.0 * cosw;
		b2 = 1;
		a0 = 1.0 + alpha;
		a1 = 2.0 * cosw;
		a2 = alpha - 1.0;
	}
	else if (type == LOWPASS){
		b0 = (1.0 - cosw) * 0.5;
		b1 = 1.0 - cosw;
		b2 = (1.0 - cosw) * 0.5;
		a0 = 1.0 + alpha;
		a1 = 2.0 * cosw;
		a2 = alpha - 1.0;
	}
	else if (type == HIGHPASS){
		b0 = (1.0 + cosw) * 0.5;
		b1 = -(cosw + 1.0);
		b2 = (1.0 + cosw) * 0.5;
		a0 = 1.0 + alpha;
		a1 = 2.0 * cosw;
		a2 = alpha - 1.0;
	}
	else if (type == LOW_SHELF){
		b0 = amp * ((amp+1.0) - ((amp-1.0)*cosw) + (beta*sinw));
		b1 = 2.0 * amp * ((amp-1.0) - ((amp+1.0)*cosw));
		b2 = amp * ((amp+1.0) - ((amp-1.0)*cosw) - (beta*sinw));
		a0 = (amp+1.0) + ((amp-1.0)*cosw) + (beta*sinw);
		a1 = 2.0F * ((amp-1.0) + ((amp+1.0)*cosw));
		a2 = -((amp+1.0) + ((amp-1.0)*cosw) - (beta*sinw));
	}
	else if (type == HIGH_SHELF){
		b0 = amp * ((amp+1.0F) + ((amp-1.0)*cosw) + (beta*sinw));
		b1 = -2.0 * amp * ((amp-1.0) + ((amp+1.0)*cosw));
		b2 = amp * ((amp+1.0) + ((amp-1.0)*cosw) - (beta*sinw));
		a0 = (amp+1.0) - ((amp-1.F)*cosw) + (beta*sinw);
		a1 = -2.0 * ((amp-1.0) - ((amp+1.0)*cosw));
		a2 = -((amp+1.0) - ((amp-1.0)*cosw) - (beta*sinw));
	}

	set(a0, a1, a2, b0, b1, b2);
}

void SecondOrderFilter::set(double a0, double a1, double a2, double b0, double b1, double b2){
	a = b0/a0;
	b = b1/a0;
	c = b2/a0;
	d = a1/a0;
	e = a2/a0;
}

float SecondOrderFilter::processOneSample(float input){
	double output = (a*input) + (b*x1) + (c*x2) + (d*y1) + (e*y2);
	x2 = x1;
	x1 = input;
	y2 = y1;
	y1 = output;
	return (float) output;
}
