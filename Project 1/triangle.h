#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vector3.h"
#include "matrix34.h"
#include "core.h"
#include <vector>
#include "particle.h"
class triangle
{
private:
	particle *a;
	particle *b;
	particle *c;
	float density;
	float drag;
public:
	void setA(particle *t) { a = t; }
	void setB(particle *t) { b = t; }
	void setC(particle *t) { c = t; }
	void setDensity(float d) { density = d; }
	void setDrag(float d) { drag = d; }
	particle getA() { return *a; }
	particle getB() { return *b; }
	particle getC() { return *c; }
	void Draw() {
		a->Draw();
		b->Draw();
		c->Draw();
	}
	void CalculateForces(){
		Vector3 velocity = a->getVelocity() + b->getVelocity() + c->getVelocity();
		velocity = velocity / 3;
		velocity = velocity - a->getWind();
		Vector3 normal;
		normal.Cross((c->getPosition() - a->getPosition()), (b->getPosition() - a->getPosition()));
		Vector3 normalizedNormal = normal;
		normalizedNormal.Normalize();
		a->addNormal(normalizedNormal);
		b->addNormal(normalizedNormal);
		c->addNormal(normalizedNormal);
		Vector3 frc = ((sqrt(velocity.Mag2() / normal.Mag2()) * velocity.Dot(normal))/2) * normal;
		frc = frc * density * drag * -1/2;
		a->ApplyForce(frc);
		b->ApplyForce(frc);
		c->ApplyForce(frc);

	}
	triangle();
	triangle(particle *a, particle *b, particle *c, float den, float dra) {
		this->a = a;
		this->b = b;
		this->c = c;
		density = den;
		drag = dra;
	}
	~triangle();
};
#endif

