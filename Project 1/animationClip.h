#ifndef ANIMATIONCLIP_H
#define ANIMATIONCLIP_H
#include <vector>
#include "DOF.h"
#include "channel.h"
class animationClip
{
private:
	std::vector<channel> channels;
	std::vector<DOF*> dofs;
	float rangeMax;
	float rangeMin;
public:
	bool Load(const char *file);
	animationClip(const char*file, std::vector < DOF*> dof);
	void evaluate(float t);
	void setRangeMax(float r) {	rangeMax = r; }
	void setRangeMin(float r) { rangeMin = r; }
	float getRangeMax() { return rangeMax; }
	float getRangeMin() { return rangeMin; }
	void setDOFs(std::vector<DOF*> d) { dofs = d; }
	void print() {
		for (int i = 0; i < channels.size(); i++) {
			channels[i].print();
		}
	}
	void preCompute() {
		for (int i = 0; i < channels.size(); i++) {
			channels[i].preLoad();
		}
	}
	animationClip(std::vector<DOF*> dofs);
	animationClip();
	~animationClip();
};

#endif