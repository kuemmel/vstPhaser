#ifndef STAGES_H
#define STAGES_H

/**
 * stages class a simple object oriented solution for returning the respective stage value
 *	- a switch, structure makes more sense and is faster.
 *
 * returns the current index for a series of filters
 * initialize with Stages stage = Stages(5); which is 12 stages, so stages.getIndex() will return the index and succ for values from 0 - 11
 * 
 **/
class Stages {
public:
	Stages();
	~Stages();

	/**
	 * Initialize the stages with a first key
	 **/
	Stages(unsigned short stage_key);

	/**
	 * Add custom keys for further development (a.i more filter-iterations/stages).
	 **/
	Stages(unsigned short &custom_keys, unsigned short cutom_keys_length, unsigned short stage_key);

	/**
	 * check wherever index is still inbound
	 * So if the stage is 4 (meaning 12) and the index is 10 it will return true and add one).
	 * Main ussage is inside a loop
	 *  
	 *  stages.setStage(2);
	 *  while(stages.checkIndex()) {
	 *     // should run 4 times
	 *  }
	 **/
	bool checkIndex();

	/**
	* Returns the current index.
	**/
	unsigned short getIndex();
	/**
	 * Returns the current stage
	 **/
	unsigned short getStage();

	/**
	 * short for resetIndex();
	 **/
	unsigned short reset();

	/**
	 * Resets the current index to 0 and returns 0.
	 **/
	unsigned short resetIndex();

	/**
	 * Sets the stage value according to the keys and resets the index.
	 * default keys:
	 * 0 ->  0 (off position, always return false)
	 * 1 ->  2
	 * 2 ->  4
	 * 3 ->  8
	 * 4 -> 12
	 **/
	void setStage(unsigned short stage_key);


private:
	//helper function, DRY
	void init(unsigned short stage_key);
	void initArray();

	unsigned short current_stage;
	unsigned short index;
	//unsigned short stage_keys[5] = { 0, 2, 4, 8, 12 };
	unsigned short * stage_keys;
	unsigned short stage_keys_length = 5;
};

#endif