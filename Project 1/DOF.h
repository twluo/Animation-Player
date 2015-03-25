#ifndef DOF_H
#define DOF_H

#include "matrix34.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
class DOF
{
private:
	float max;
	float min;
	float value;
	std::string name;
	std::string tag;
public:
	DOF();
	DOF(std::string name, std::string tag) { this->name = name; this->tag = tag; }
	~DOF();
	std::string getName() { return name; }
	void setName(std::string name) { this->name = name; }
	void setBound(float a, float b) { max = a; min = b; }
	float getMax() { return max; }
	float getMin() { return min; }
	std::string getTag() { return tag; }
	void setTag(std::string t) { tag = t; }
	void setValue(float a) {
		if (max > a)
			if (a > min) {
				value = a;
			}
		if (max <= a)
			value = max;
		if (a <= min)
			value = min;
	}
	float getValue() { return value; }

	void print() { std::cout << tag << " " << "MAX = " << max << " MIN = " << min << " VALUE = " << value << std::endl; }
};

#endif