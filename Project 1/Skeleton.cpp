#include "Skeleton.h"


Skeleton::Skeleton()
{

}
Skeleton::Skeleton(const char *file) {
	Load(file);
	Matrix34 id;
	x = new DOF();
	y = new DOF();
	z = new DOF();
	x->setName("Root Translation");
	y->setName("Root Translation");
	z->setName("Root Translation");
	x->setTag("x");
	y->setTag("y");
	z->setTag("z");
	joints.push_back(x);
	joints.push_back(y);
	joints.push_back(z);
	calculate(id.IDENTITY);
	getMatrixesAndJoints();
	//print();
	//printf("There are %i matrixes \n", worldMatrixes.size());
	//printf("There are %i joints \n", joints.size());
}

Skeleton::~Skeleton()
{
}

bool Skeleton::Load(const char *file) {
	Tokenizer token;
	token.Open(file);
	token.FindToken("balljoint");
	// Parse tree
	Root = new Joint("Root");
	Root->Load(token);
	// Finish
	token.Close();
	return true;
}