#ifndef SKELETON_H
#define SKELETON_H


#include "token.h"
#include "joint.h"
#include "DOF.h"
#include "Matrix34.h"
#include <vector>
class Skeleton
{
private:
	Joint *Root;
public:
	Skeleton();
	DOF* x;
	DOF* y;
	DOF* z;
	Matrix34 translation;
	std::vector<Matrix34*> worldMatrixes;
	std::vector<DOF*> joints;
	Skeleton(const char *file);
	bool Load(const char *file);
	void calculate(Matrix34 m) { translation.MakeTranslate(Vector3(x->getValue(), y->getValue(), z->getValue())); m.Dot(translation, m); Root->calculate(m); }
	void draw() { Root->draw(); }
	void print() { Root->print(); }
	void reset() { Root->reset(); x->setValue(0); y->setValue(0); z->setValue(0); }
	void getMatrixesAndJoints() { Root->getMatrixesAndJoints(worldMatrixes, joints); }
	~Skeleton(); 
};

#endif