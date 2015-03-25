#ifndef CHANNEL_H
#define CHANNE_H
#include <vector>
#include "keyframe.h"
#include "token.h"
#include <string>
#include <iostream>
class channel
{
private:
	std::vector<keyframe> keyframes;
	std::string extrapolateInMode;
	std::string extrapolateOutMode;
	int lastIndex = 0;
	float length;
	keyframe start;
	keyframe end;

public:
	void setExtraPolateInMode(std::string extra) {
		extrapolateInMode = extra;
	}
	void setExtraPolateOutMode(std::string extra) {
		extrapolateOutMode = extra;
	}
	std::string getExtrapolateInMode() {
		return extrapolateInMode;
	}
	std::string getExtrapolateOutMode() {
		return extrapolateOutMode;
	}
	int number;
	void preLoad();
	void print() {
		std::cout << "extraIn = " << extrapolateInMode << " extraOut = " << extrapolateOutMode << std::endl;
		for (int i = 0; i < keyframes.size(); i++) {
			keyframes[i].print();
		}
	}
	float evaluate(float t);
	bool Load(Tokenizer &t);
	channel();
	~channel();
};

#endif
