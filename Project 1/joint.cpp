#include "joint.h"


Joint::Joint()
{
}

bool Joint::Load(Tokenizer &token) {
	token.FindToken("{");
	Vector3 offset;
	Vector3 boxmin;
	Vector3 boxmax;
	float min;
	float max;
	Vector3 pose;
	while (1) {
		char temp[256];
		token.GetToken(temp);
		if (strcmp(temp, "offset") == 0) {
			offset.x = token.GetFloat();
			offset.y = token.GetFloat();
			offset.z = token.GetFloat();
			setTranslation(offset);
		}
		else if (strcmp(temp, "boxmin") == 0) {
			boxmin.x = token.GetFloat();
			boxmin.y = token.GetFloat();
			boxmin.z = token.GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0) {
			boxmax.x = token.GetFloat();
			boxmax.y = token.GetFloat();
			boxmax.z = token.GetFloat();
			setScale(boxmax, boxmin);
		}
		else if (strcmp(temp, "rotxlimit") == 0) {
			min= token.GetFloat();
			max = token.GetFloat();
			setDOFBoundX(max, min);
		}
		else if (strcmp(temp, "rotylimit") == 0) {
			min = token.GetFloat();
			max = token.GetFloat();
			setDOFBoundY(max, min);
		}
		else if (strcmp(temp, "rotzlimit") == 0) {
			min = token.GetFloat();
			max = token.GetFloat();
			setDOFBoundZ(max, min);
		}
		else if (strcmp(temp, "pose") == 0) {
			pose.x = token.GetFloat();
			pose.y = token.GetFloat();
			pose.z = token.GetFloat();
			setPose(pose);
		}
			// Check for other tokens
		else if (strcmp(temp, "balljoint") == 0) {
			char name[256];
			token.GetToken(name);
			//std::cout << name << std::endl;
			Joint *jnt = new Joint(name);
			jnt->Load(token);
			addChild(*jnt);
		}
		else if (strcmp(temp, "}") == 0) return true;
		else token.SkipLine(); // Unrecognized token
	}
}
Joint::~Joint()
{
}
