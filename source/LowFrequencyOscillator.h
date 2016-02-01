/**
 *
 * Class to implement a Low frequency oscillator
 **/

class LowFrequencyOscillator {
public:
	/**
	 * Initializes the oscillator with defaults:
	 *	 sampleRate = 44100
	 *	 frequency = 100 Hz
	 *	 gain = 1
	 **/
	LowFrequencyOscillator();

	/**
	* Initializes the oscillator with values:
	*	 @param sampleRate	see sampling rate on wikipedia, typical values are 44100Hz, 48000 Hz or 96000Hz
	*	 @param gain		default is one, this is linnear! [0..1]!
	*	 @param frequency	the starting frequency, recommended values (as this is an LFO) [0..20] Hz
	**/
	LowFrequencyOscillator(double sampleRate,double gain,double frequency);

	~LowFrequencyOscillator(){};

	/**
	* (also get() )
	* Return current value of the function(index moves one further)
	* @return x [-gain..gain] a value of a sinus function according to the values
	**/
	double process();
	/**
	 * Return current value of the function(index moves one further)
	 * @return x [-gain..gain] a value of a sinus function according to the values
	 **/
	double get();

	/**
	 * set sampling rate, typical values are 44100Hz, 48000 Hz or 96000Hz
	 *	@param sampleRate	samplingRate of the oscillator
	 **/
	void setSampleRate(double sampleRate);

	/**
	* set the gain, linnear! [0..1], 1 being the standard amplitude.
	* @apram gain	gain from 0..1
	**/
	void setGain(double gain);

	/**
	* the the Frequency of the LFO
	* @param frequency	the frequency, recommended values (as this is an LFO) [0..20] Hz, values over 20kHz don't make sense.
	**/
	void setFrequency(double frequency);

	double getSampleRate();
	double getGain();
	double getFrequency();

private:
	double sampleRate;
	double gain;
	double frequency;
	double x;
	int index;
};