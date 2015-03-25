#ifndef SPRINGDAMPER_H
#define SPRINGDAMPER_H
#include "vector3.h"
#include "particle.h"
class springDamper
{
private:
	particle *P1;
	particle *P2;
	float SpringConstant;
	float DampingFactor;
	float RestLength;
public:
	void setSpringConstant(float sc) { SpringConstant = sc; }
	void setDampingFactor(float df) { DampingFactor = df; }
	float getSpringConstant() { return SpringConstant; }
	float getDampingFactor() { return DampingFactor;  }
	springDamper(particle* p, particle *q, float sc, float dp) {
		P1 = p;
		P2 = q;
		SpringConstant = sc;
		DampingFactor = dp;
		Vector3 length = P2->getPosition() - P1->getPosition();
		RestLength = length.Mag();
	}
	void CalculateForces() {
		Vector3 unitLength = P2->getPosition() - P1->getPosition();
		float length = unitLength.Mag();
		unitLength = unitLength / length;
		float deltaSpeed = P1->getVelocity().Dot(unitLength) - P2->getVelocity().Dot(unitLength);
		float deltaLength = RestLength - length;
		float sdForce = -(SpringConstant * deltaLength) - (DampingFactor * deltaSpeed);
		Vector3 F = sdForce * unitLength;
		//F.Print("F");
		P1->ApplyForce(F);
		P2->ApplyForce(-F);
		Draw();
	}
	void Draw() {
		Matrix34 id;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(id);
		glBegin(GL_LINES);
		glColor3d(0, 1, 1);
		glVertex3f(P1->getPosition().x, P1->getPosition().y, P1->getPosition().z);
		glVertex3f(P2->getPosition().x, P2->getPosition().y, P2->getPosition().z);
		glEnd();
	}
	springDamper();
	~springDamper();
};

#endif