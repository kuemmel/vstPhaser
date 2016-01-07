#include "Stages.h"
#include <stdexcept>
#include <algorithm>
#include <iterator>

/**
 * For documentation see stages.h
 **/
Stages::Stages() {
	initArray();
	init(0);
}

Stages::Stages(unsigned short stage_key) {
	initArray();
	init(stage_key);
}

void Stages::initArray() {
	unsigned short stage_keys[5] = { 0, 2, 4, 8, 12 };
	this->stage_keys = new unsigned short[5]();
	for (int i = 0; i < 4; i++) {
		this->stage_keys[i] = stage_keys[i];
	}
}

Stages::Stages(unsigned short &custom_keys, unsigned short cutom_keys_length, unsigned short stage_key) {
	this->stage_keys_length = cutom_keys_length;
	this->stage_keys = &custom_keys;

	init(stage_key);
}

void Stages::init(unsigned short stage_key) {
	if (stage_key <= (this->stage_keys_length)) {
		this->current_stage = stage_keys[stage_key];
		this->index = 0;
	}
	else {
		throw std::invalid_argument("stage_key out bounds(should be between 0 and 4)");
	}
}

bool Stages::checkIndex() {
	if (this->index < this->current_stage) {
		this->index++;
		return true;
	}
	else {
		return false;
	}
}

unsigned short Stages::getIndex() {
	return this->index;
}

unsigned short Stages::reset() {
	return resetIndex();
}

unsigned short Stages::resetIndex() {
	this->index = 0;
	return 0;
}

void Stages::setStage(unsigned short stage_key) {
	this->current_stage = stage_keys[stage_key];
}