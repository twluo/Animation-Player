#ifndef CLOTH_H
#define CLOTH_H
#include "vector3.h"
#include "matrix34.h"
#include "core.h"
#include <vector>
#include "triangle.h"
#include "particle.h"
#include "springDamper.h"
class cloth
{
private:
	float DampingFactor;
	float SpringConstant;
	float density;
	float drag;
	Vector3 wind;
public:
	void setWind(Vector3 w) { wind = w; }
	void setDensity(float d) { density = d; }
	void setDrag(float d) { drag = d; }
	void setSpringConstant(float sc) { SpringConstant = sc; }
	void setDampingFactor(float df) { DampingFactor = df; }
	float getSpringConstant() { return SpringConstant; }
	float getDampingFactor() { return DampingFactor; }
	std::vector<std::vector<particle*>> particles;
	std::vector<triangle> triangles;
	std::vector<springDamper> springDampers;
	void setTriangles() {
		for (int i = 0; i < particles.size(); i++) {
			for (int j = 0; j < particles[i].size() - 1; j++) {
				if (i == 0) {
					triangles.push_back(triangle(particles[i][j], particles[i][j + 1], particles[i + 1][j], density, drag));
					//printf("Made this Triangle \n");
					//particles[i][j]->getPosition().Print("A");
					//particles[i][j + 1]->getPosition().Print("B");
					//particles[i + 1][j]->getPosition().Print("C");
				}
				else if (i == particles.size() - 1) {
					triangles.push_back(triangle(particles[i][j], particles[i - 1][j + 1], particles[i][j + 1], density, drag));
					//printf("Made this Triangle \n");
					//particles[i][j]->getPosition().Print("A");
					//particles[i-1][j + 1]->getPosition().Print("B");
					//particles[i][j + 1]->getPosition().Print("C");
				}
				else {
					triangles.push_back(triangle(particles[i][j], particles[i][j + 1], particles[i + 1][j], density, drag));
					//printf("Made this Triangle \n");
					//particles[i][j]->getPosition().Print("A");
					//particles[i][j + 1]->getPosition().Print("B");
					//particles[i + 1][j]->getPosition().Print("C");

					triangles.push_back(triangle(particles[i][j], particles[i - 1][j + 1], particles[i][j + 1], density, drag));
					//printf("Made this Triangle \n");
					//particles[i][j]->getPosition().Print("A");
					//particles[i - 1][j + 1]->getPosition().Print("B");
					//particles[i][j + 1]->getPosition().Print("C");
				}
			}
		}/*
		for (int i = 0; i < particles.size(); i++) {
			for (int j = 0; j < particles[i].size(); j++) {
				particles[i][j]->getPosition().Print("P");
			}
		}*/
	}
	void setSpringDampers() {
		for (int i = 0; i < particles.size(); i++) {
			for (int j = 0; j < particles[i].size(); j++) {
				if (i == 0) {
					if (j == particles[i].size() - 1) {
						springDampers.push_back(springDamper(particles[i][j], particles[i + 1][j], SpringConstant, DampingFactor));
					}
					else {
						springDampers.push_back(springDamper(particles[i][j], particles[i][j + 1], SpringConstant, DampingFactor));
						springDampers.push_back(springDamper(particles[i][j], particles[i + 1][j + 1], SpringConstant, DampingFactor));
						springDampers.push_back(springDamper(particles[i][j], particles[i + 1][j], SpringConstant, DampingFactor));
					}
				}
				else if (i == particles.size() - 1) {
					if (j != particles[i].size() - 1) {
						springDampers.push_back(springDamper(particles[i][j], particles[i - 1][j + 1], SpringConstant, DampingFactor));
						springDampers.push_back(springDamper(particles[i][j], particles[i][j + 1], SpringConstant, DampingFactor));
					}
				}
				else {
					if (j == particles[i].size() - 1){
						springDampers.push_back(springDamper(particles[i][j], particles[i + 1][j], SpringConstant, DampingFactor));
					}
					else {
						springDampers.push_back(springDamper(particles[i][j], particles[i - 1][j + 1], SpringConstant, DampingFactor));
						springDampers.push_back(springDamper(particles[i][j], particles[i][j + 1], SpringConstant, DampingFactor));
						springDampers.push_back(springDamper(particles[i][j], particles[i + 1][j + 1], SpringConstant, DampingFactor));
						springDampers.push_back(springDamper(particles[i][j], particles[i + 1][j], SpringConstant, DampingFactor));
					}
				}
			}
		}
		springDampers.push_back(springDamper(particles[0][0], particles[0][particles[0].size() -1], 100, DampingFactor));
		springDampers.push_back(springDamper(particles[0][0], particles[particles.size() - 1][0], 100, DampingFactor));
		springDampers.push_back(springDamper(particles[0][0], particles[particles.size()-1][particles[0].size() - 1], 100, DampingFactor));
		springDampers.push_back(springDamper(particles[particles.size() - 1][0], particles[0][particles[0].size() -1 ], 100, DampingFactor));
		springDampers.push_back(springDamper(particles[particles.size() - 1][0], particles[particles.size() - 1][particles[0].size() - 1], 100, DampingFactor));
		springDampers.push_back(springDamper(particles[particles.size() - 1][particles[0].size() - 1], particles[0][particles[0].size() - 1], 100, DampingFactor));

		printf("%d\n", springDampers.size());
	}
	void Update(float deltaTime) {
		for (int i = 1; i < particles.size(); i++) {
			for (int j = 0; j < particles[i].size(); j++) {
				particles[i][j]->ApplyForce(Vector3(0, -9.8, 0) * particles[i][j]->getMass());
				particles[i][j]->ApplyForce(wind);
				//particles[i][j]->Update(deltaTime);
			}
		}
		for (int i = 0; i < springDampers.size(); i++) {
			springDampers[i].CalculateForces();
		}
		for (int i = 0; i < triangles.size(); i++) {
			triangles[i].CalculateForces();
		}
		for (int i = 0; i < particles.size(); i++) {
			for (int j = 0; j < particles[i].size(); j++) {
				particles[i][j]->Update(deltaTime);
			}
		}

	}
	void Draw() {
		Matrix34 id;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(id);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < triangles.size(); i++) {
			triangles[i].Draw();
		}
		glEnd();
		for (int i = 0; i < particles.size(); i++) {
			for (int j = 0; j < particles[i].size(); j++) {
				particles[i][j]->resetNormals();
			}
		}
		/*
		for (int i = 0; i < springDampers.size(); i++) {
			springDampers[i].Draw();
		}
		*/
	}

	cloth();
	~cloth();
};

#endif