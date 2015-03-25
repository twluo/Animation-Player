#ifndef PARTICLE_H
#define PARTICLE_H
#include "vector3.h"
#include "matrix34.h"
#include "core.h"
#include <vector>
class particle
{
private:
	float mass;
	Vector3 position;
	Vector3 velocity;
	Vector3 force;
	bool Static;
	Vector3 wind;
	std::vector<Vector3> normals;
public:
	void moveForward() { position.z += 0.1; }
	void moveBackward() { position.z -= 0.1; }
	void moveUp() { position.y += 0.1; }
	void moveDown() { position.y -= 0.1; }
	void moveRight() { position.x += 0.1; }
	void moveLeft() { position.x -= 0.1; }
	void setStatic(bool s) { Static = s; }
	bool getStatic() { return Static; }
	void setMass(float m) { mass = m; }
	float getMass() { return mass; }
	void setPosition(Vector3 pos) {	position = pos; }
	Vector3 getPosition() { return position; }
	void setVelocity(Vector3 vel) { velocity = vel; }
	Vector3 getVelocity() { return velocity; }
	void setForce(Vector3 forc) { force = forc; }
	Vector3 getForce() { return force; }
	void ApplyForce(Vector3 f) { force = force + f; }
	void setWind(Vector3 w) { wind = w; }
	Vector3 getWind() { return wind; }
	void addNormal(Vector3 normal) { normals.push_back(normal); }
	void Draw() {
		glColor3d(1, 0, 0);
		Vector3 normal;
		for (int i = 0; i < normals.size(); i++) {
			normal += normals[i];
		}
		normal = normal / normals.size();
		normal.Normalize();
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(position.x, position.y, position.z);
	}
	void Update(float deltaTime) {
		if (!Static) {
			Vector3 accel = (1.0 / mass) * force;
			velocity += accel * deltaTime;
			position += velocity * deltaTime;
			force = Vector3(0, 0, 0);
			if (position.y < -10) {
				position.y = -9.5;
				velocity.y = 0.2 * velocity.y;
				velocity.x = 0.2 * velocity.x;
				velocity.z = 0.2 * velocity.z;
			}
		}
	}
	particle();
	particle(Vector3 pos){
		position = pos;
	}
	void resetNormals() { normals.clear(); }
	particle(Vector3 pos, float m, bool sta) {
		position = pos;
		mass = m;
		Static = sta;
	}
	~particle();
};

#endif