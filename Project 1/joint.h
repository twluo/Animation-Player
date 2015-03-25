#ifndef JOINT_H
#define JOINT_H

#include "DOF.h"
#include "matrix34.h"
#include <string>
#include <vector>
#include <iostream>
#include "token.h"
#include "core.h"
class Joint
{
private: 
	std::string name;
	DOF *dofx = new DOF();
	DOF *dofy = new DOF();
	DOF *dofz = new DOF();
	Matrix34 *localMatrix = new Matrix34();
	Matrix34 *scale = new Matrix34();
	Matrix34 *translation = new Matrix34();
	Matrix34 *rotationX = new Matrix34();
	Matrix34 *rotationY = new Matrix34();
	Matrix34 *rotationZ = new Matrix34;
	Matrix34 *worldMatrix = new Matrix34();
	Vector3 offset;
	Vector3 minBox;
	Vector3 maxBox;
	Vector3 pose;
	std::vector<Joint> child;
	int position;

public:
	Joint();
	Joint(std::string n) {	name = n; dofx->setName(n); dofy->setName(n); dofz->setName(n);  dofz->setTag("z"); dofy->setTag("y"); dofx->setTag("x");
	localMatrix->Identity(); scale->Identity(); translation->Identity(); rotationX->Identity(); rotationY->Identity(); rotationZ->Identity(); position = 0;
	}

	std::string getName() { return name; }

	//Setting DOFs Bounds
	void setDOFBoundX(float max, float min) { dofx->setBound(max, min); }
	void setDOFBoundY(float max, float min) { dofy->setBound(max, min); }
	void setDOFBoundZ(float max, float min) { dofz->setBound(max, min); }

	//Setting DOFs
	void setDOFX(float value) { dofx->setValue(value);  this->pose.x = value;	}
	void setDOFY(float value) { dofy->setValue(value);  this->pose.y = value;	}
	void setDOFZ(float value) { dofz->setValue(value);  this->pose.z = value;	}
	void setPose(Vector3 pose) { setDOFX(pose.x); setDOFY(pose.y); setDOFZ(pose.z); }

	//Setting scaling matrix
	void setScale(Vector3 max, Vector3 min) {
		maxBox = max;
		minBox = min;
		Vector3 scalar = max - min;
		scale->MakeScale(scalar);
	}
	void reset() {
		setPose(pose);
		for (unsigned int i = 0; i < child.size(); i++) {
			Joint temp = child[i];
			temp.reset();
		}
	}
	//Setting translation
	void setTranslation(Vector3 move) {
		offset = move;	
		translation->MakeTranslate(move);
	}

	void calculate(Matrix34 m) {
		localMatrix->Identity();
		localMatrix->Dot(*localMatrix, *translation);
		rotationZ->MakeRotateZ(dofz->getValue());
		localMatrix->Dot(*localMatrix, *rotationZ);
		rotationY->MakeRotateY(dofy->getValue());
		localMatrix->Dot(*localMatrix, *rotationY);
		rotationX->MakeRotateX(dofx->getValue());
		localMatrix->Dot(*localMatrix, *rotationX);
		worldMatrix->Dot(m, *localMatrix);
		//std::cout <<"I'M " << getName() << " AND I JUST GOT CALCULATED "<<  std::endl;
		//localMatrix->Print();
		for (unsigned int i = 0; i < child.size(); i++) {
			Joint temp = child[i];
			temp.calculate(*worldMatrix);
		}
	}

	void draw() {
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(*worldMatrix);
		//localMatrix->Print();
		drawWireBox(minBox.x, minBox.y, minBox.z, maxBox.x, maxBox.y, maxBox.z);
		for (unsigned int i = 0; i < child.size(); i++) {
			Joint temp = child[i];
			temp.draw();
		}
	}

	//Add Child to list
	void addChild(Joint c) {
		child.push_back(c);
	}

	bool Load(Tokenizer &t);

	void getMatrixesAndJoints(std::vector<Matrix34*> &worldMatrixes, std::vector<DOF*> &joints) {
		worldMatrixes.push_back(worldMatrix);
		joints.push_back(dofx);
		joints.push_back(dofy);
		joints.push_back(dofz);
		for (unsigned int i = 0; i < child.size(); i++) {
			Joint temp = child[i];
			temp.getMatrixesAndJoints(worldMatrixes, joints);
		}
	}
	void print() {
		std::cout << "NAME IS " << this->getName() << std::endl;
		std::cout << "OFFSET IS " << offset.x << ", " << offset.y << ", " << offset.z << std::endl;
		std::cout << "MINBOX IS " << minBox.x << ", " << minBox.y << ", " << minBox.z << std::endl;
		std::cout << "MAXBOX IS " << maxBox.x << ", " << maxBox.y << ", " << maxBox.z << std::endl;
		std::cout << "POSE IS " << pose.x << ", " << pose.y << ", " << pose.z << std::endl;
		std::cout << "I HAVE " << child.size() << " Children" << std::endl;
		dofx->print();
		dofy->print();
		dofz->print();
		std::cout << std::endl;
		for (unsigned int i = 0; i < child.size(); i++) {
			Joint temp = child[i];
			temp.print();
		}
	}
	~Joint();
};

#endif