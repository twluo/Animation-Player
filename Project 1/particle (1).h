#ifndef PARTICLE_H
#define PARTICLE_H
#include "vector3.h"
#include "matrix34.h"
#include "core.h"
class particle
{
private:
	float mass;
	Vector3 position;
	Vector3 velocity;
	Vector3 force;
public:
	void setMass(float m) { mass = m; }
	float getMass() { return mass; }
	void setPosition(Vector3 pos) {	position = pos; }
	Vector3 getPosition() { return position; }
	void setVelocity(Vector3 vel) { velocity = vel; }
	Vector3 getVelocity() { return velocity; }
	void setForce(Vector3 forc) { force = forc; }
	Vector3 getForce() { return force; }
	void ApplyForce(Vector3 f) { force = force + f; }
	void Draw() {
		Matrix34 id;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(id);
		glBegin(GL_QUADS);
		glColor3d(1, 0, 0);
		glVertex3f(position.x + 2, position.y - 2, position.z);
		glVertex3f(position.x + 2, position.y + 2, position.z);
		glVertex3f(position.x - 2, position.y + 2, position.z);
		glVertex3f(position.x - 2, position.y - 2, position.z);
		glEnd();
	}
	void Update(float deltaTime) {
		Vector3 accel = (1.0 / mass) * force;
		velocity += accel * deltaTime;
		position += velocity * deltaTime;
		force = Vector3(0, 0, 0);
	}
	particle();
	~particle();
};

#endif