#ifndef ALLPASS_H
#define ALLPASS_H

struct NotchFilter {
	float a;
	float b;
	float d[3];
	float x[3];
	float y[3];
};

/**
 * Simple second order Allpass-filter implementation based on http://www.dsprelated.com/
 * 
 **/
class AllpassFilter {
public:
	AllpassFilter(){};
	~AllpassFilter();

	/**
	 * calculates the coefficient array from the given inputs
	 * @sample_rate			- usual sample rate is 44100Hz
	 * @cut_of_frequency	- gain at cut frequencies
	 * @frequency_step		- step to increase the center frequencies
	 * @minimum_frequency	- minimum center frequency
	 * @Q					- Q, Quality factor, see filter
	 **/
	AllpassFilter(float sample_rate, float new_gain, float frquency_step, float minimum_frequency, float Q);

	/**
	* loads the coefficients into the Filter object
	* @center_frequency		- frequency of the notch
	**/
	void init(int notch_frequency);

	/**
	 * Actually processes the filter on to the input signal
	 **/
	float process(float input);

	/**
	 * Returns the filter object (last results, inputs and parameters) as a NotchFilter struct
	 *
	 **/
	struct NotchFilter getFilterObject();

private:
	int max_coefficients = 100;
	float **coefficients;
	float sampleRate;

	struct NotchFilter H;
};

#endif