#ifndef KEYFRAME_H
#define KEYFRAME_H
#include <string>
#include <iostream>
class keyframe
{
private:
	float time;
	float value;
	float tangentInValue;
	float tangentOutValue;
	std::string tangentInMode;
	std::string tangentOutMode;
	float A, B, C, D;
public:
	float getTime() {
		return time;
	}
	void setTime(float t) {
		time = t;
	}

	float getValue() {
		return value;
	}
	void setValue(float v) {
		value = v;
	}

	float getTangentInValue() {
		return tangentInValue;
	}
	float getTangentOutValue() {
		return tangentOutValue;
	}
	void setTangentInValue(float tangent) {
		tangentInValue = tangent;
	}
	void setTangentOutValue(float tangent) {
		tangentOutValue = tangent;
	}
	void setLinearTangentIn(keyframe prev) {
		tangentInValue = ((value - prev.getValue()) / (time - prev.getTime()));
	}
	void setLinearTangentOut(keyframe next) {
		tangentOutValue = ((next.getValue() - value) / (next.getTime() - time));
	}
	void setSmoothTangent(keyframe prev, keyframe next) {
		tangentInValue = ((next.getValue() - prev.getValue()) / (next.getTime() - prev.getTime()));
		tangentOutValue = tangentInValue;
	}
	std::string getTangentInMode() {
		return tangentInMode;
	}
	std::string getTangentOutMode() {
		return tangentOutMode;
	}
	void setTangentInMode(std::string tangentMode) {
		tangentInMode = tangentMode;
	}
	void setTangentOutMode(std::string tangentMode) {
		tangentOutMode = tangentMode;
	}
	void printABCD() {
		std::cout << "A = " << A << " B = " << B << " C = " << C << " D = " << D << std::endl;
	}
	void calculateCoeficients(keyframe next) {
		float newTangent = (next.getTime() - time) * tangentOutValue;
		float newNextTangent = (next.getTime() - time) * next.tangentOutValue;
		A = 2 * value + -2 * next.getValue() + newTangent + newNextTangent;
		B = -3 * value + 3 * next.getValue() + -2 * newTangent + -1 * newNextTangent;
		C = newTangent;
		D = value;
	}
	float evaluate(float evalTime, keyframe next) {
		float newTime = (evalTime - time) / (next.getTime() - time);
		//std::cout << newTime << std::endl;
		return (D + newTime*(C + newTime *(B + newTime *(A))));
	}

	void print() {
		printf("Time = %f, Value = %f", time, value);
		std::cout << ", tangentIn = " << tangentInMode << ", tangentOut = " << tangentOutMode << std::endl;
	}
	keyframe();
	~keyframe();
};
#endif

