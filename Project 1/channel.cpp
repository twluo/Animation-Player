#include "channel.h"


channel::channel()
{
	keyframes = std::vector<keyframe>();
}


channel::~channel()
{
}

void channel::preLoad() {
	for (int i = 0; i < keyframes.size(); i++) {
		if (keyframes[i].getTangentInMode().compare("flat") == 0) {
			keyframes[i].setTangentInValue(0);
		}
		else if (keyframes[i].getTangentInMode().compare("linear") == 0) {
			if (i == 0)
				keyframes[i].setTangentInValue(0);
			else
				keyframes[i].setLinearTangentIn(keyframes[i - 1]);
		}
		else if (keyframes[i].getTangentInMode().compare("smooth") == 0) {
			if (i == 0 && i == keyframes.size() - 1) {
				keyframes[i].setTangentInValue(0);
				keyframes[i].setTangentOutValue(0);
			}
			else if (i == 0) {
				keyframes[i].setTangentInValue(0);
				keyframes[i].setLinearTangentOut(keyframes[i + 1]);
			}
			else if (i == keyframes.size() - 1) {
				keyframes[i].setLinearTangentIn(keyframes[i - 1]);
				keyframes[i].setTangentOutValue(0);
			}
			else
				keyframes[i].setSmoothTangent(keyframes[i - 1], keyframes[i + 1]);
		}
		if (keyframes[i].getTangentOutMode().compare("flat") == 0) {
			keyframes[i].setTangentOutValue(0);
		}
		else if (keyframes[i].getTangentOutMode().compare("linear") == 0) {
			if (i == keyframes.size() - 1)
				keyframes[i].setTangentOutValue(0);
			else
				keyframes[i].setLinearTangentOut(keyframes[i + 1]);
		}
	}
	for (int i = 0; i < keyframes.size() - 1; i++) {
		keyframes[i].calculateCoeficients(keyframes[i + 1]);
	}
	//for (int i = 0; i < keyframes.size(); i++) {
	//	keyframes[i].printABCD();
		//std::cout << "tanIn = " << keyframes[i].getTangentInValue() << " tanOut = " << keyframes[i].getTangentOutValue() << std::endl;
	//}
	start = keyframes[0];
	end = keyframes[keyframes.size() - 1];
	length = keyframes[keyframes.size() - 1].getTime() - start.getTime();
	
}
bool channel::Load(Tokenizer &token) {
	token.FindToken("{");
	char temp[256];
	token.GetToken(temp);
	if (strcmp(temp, "extrapolate") == 0) {
		token.GetToken(temp);
		extrapolateInMode = temp;
		token.GetToken(temp);
		extrapolateOutMode = temp;
	}
	token.GetToken(temp);
	if (strcmp(temp, "keys") == 0) {
		int index = token.GetInt();
		token.FindToken("{");
		for (int i = 0; i < index; i++) {
			keyframe *key = new keyframe();
			key->setTime(token.GetFloat());
			key->setValue(token.GetFloat());
			token.GetToken(temp);
			if (isdigit(temp[0])) {
				key->setTangentInValue(atof(temp));
				key->setTangentOutMode("constant value");
			}
			else
				key->setTangentInMode(temp);
			token.GetToken(temp);
			if (isdigit(temp[0])) {
				key->setTangentOutValue(atof(temp));
				key->setTangentOutMode("constant value");
			}
			else
				key->setTangentOutMode(temp);
			keyframes.push_back(*key);
		}
		token.FindToken("}");
		token.FindToken("}");
	}
	return true;
}
float channel::evaluate(float t) {
//	std::cout << "t = " << t << std::endl;
	//std::cout << "start = " << start.getTime() << "index = " << number << std::endl;
	if (t < start.getTime()) {
		if (extrapolateInMode.compare("constant") == 0) {
			return start.getValue();
		}
		else if (extrapolateInMode.compare("linear") == 0) {
			float offset = start.getValue() - (start.getTime() * start.getTangentOutValue());
			return t * start.getTangentOutValue() + offset;
		}
		else if (extrapolateInMode.compare("cycle") == 0) {

			float newT = fmod(end.getTime() - t, length);
			newT = end.getTime() - newT;
			return evaluate(newT);
		}
		else if (extrapolateInMode.compare("cycle_offset") == 0) {
			float newT = fmod(end.getTime() - t, length);
			newT = end.getTime() - newT;
			int offset = (end.getTime() - t) / length;
			return evaluate(newT) - offset * (end.getValue() - start.getValue());
		}
		else if (extrapolateInMode.compare("bounce") == 0) {
			//TODO BOUNCE!!!
			float newT = fmod(end.getTime() - t, length);
			newT = end.getTime() - newT;
			int drctn = (t - start.getTime()) / length;
			if (drctn % 2 == 0)
				return evaluate(end.getTime() - newT);
			else
				return evaluate(newT);
		}
	}
	else if (t > end.getTime()) {
		if (extrapolateOutMode.compare("constant") == 0) {
			return end.getValue();
		}
		else if (extrapolateInMode.compare("linear") == 0) {
			float offset = end.getValue() - (end.getTime() * end.getTangentInValue());
			return t * end.getTangentInValue() + offset;
		}
		else if (extrapolateInMode.compare("cycle") == 0) {
			float newT = fmod(t - start.getTime(), length) + start.getTime();
			return evaluate(newT);
		}
		else if (extrapolateInMode.compare("cycle_offset") == 0) {
			float newT = fmod(t - start.getTime(), length);
			newT = newT + start.getTime();
			int offset = (t - start.getTime()) / length;
			return evaluate(newT) + offset * (end.getValue() - start.getValue());
		}
		else if (extrapolateInMode.compare("bounce") == 0) {
			//TODO BOUNCE!!!
			float newT = fmod(t - start.getTime(), length);
			newT = newT	+ start.getTime();
			int drctn = (t - start.getTime()) / length;
			if (drctn % 2 == 1)
				return evaluate(end.getTime() - newT);
			else
				return evaluate(newT);
		}
	}
	else {
		for (int i = 0; i < keyframes.size(); i++) {
			if (t < keyframes[i].getTime()) {
				return keyframes[i - 1].evaluate(t, keyframes[i]);
			}
			else if (t == keyframes[i].getTime())
				return keyframes[i].getValue();
		}
	}
}