#include "animationClip.h"


animationClip::animationClip()
{
}

animationClip::animationClip(std::vector<DOF*> d) {
	dofs = d;
}
animationClip::animationClip(const char *file, std::vector<DOF*> d) {
	dofs = d;
	Load(file);
	preCompute();
	//print();
}
void animationClip::evaluate(float t) {
	for (int i = 0; i < channels.size(); i++) {
		dofs[i]->setValue(channels[i].evaluate(t));
	}
}
bool animationClip::Load(const char *file) {
	Tokenizer token;
	token.Open(file);
	int index = 0;
	token.FindToken("range");
	rangeMin = token.GetFloat();
	rangeMax = token.GetFloat();
	//printf("%f, %f", rangeMin, rangeMax);
	while (token.FindToken("channel")) {
		channel *temp = new channel();
		temp->Load(token);
		temp->number = index;
		index++;
		channels.push_back(*temp);
	}
	// Parse tree
	//Root = new Joint("Root");
	//Root->Load(token);
	// Finish
	token.Close();
	//for (int i = 0; i < dofs.size(); i++) {
	//	std::cout << dofs[i]->getName() << " " << dofs[i]->getTag() << " " << dofs[i]->getValue() <<std::endl;
	//}
	return true;
}
animationClip::~animationClip()
{
}
