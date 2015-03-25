#ifndef SKIN_H
#define SKIN_H

#include "token.h"
#include "vector3.h"
#include <vector>
#include "skinWeight.h"
#include "matrix34.h"
#include "skeleton.h"
class skin
{
private:
	std::vector<Vector3> positions = std::vector<Vector3>();
	std::vector<Vector3> normals = std::vector<Vector3>();
	std::vector<Vector3> blendedPositions = std::vector<Vector3>();
	std::vector<Vector3> blendedNormals = std::vector<Vector3>();
	std::vector<std::vector<skinWeight>> skinWeights = std::vector<std::vector<skinWeight>>();
	std::vector<Vector3> triangles = std::vector<Vector3>();
	std::vector<Matrix34> matrixes = std::vector<Matrix34>();
	std::vector<Matrix34> invertedMatrixes = std::vector<Matrix34>();
	std::vector<Matrix34> skinningMatrixes = std::vector<Matrix34>();
	std::vector<Matrix34*> jointMatrixes = std::vector<Matrix34*>();
public:
	skin();
	void findInverses() {
		Matrix34 temp;
		invertedMatrixes.clear();
		for (int i = 0; i < matrixes.size(); i++) {
			temp = matrixes[i];
			temp.FastInverse();
			invertedMatrixes.push_back(temp);
		}
	}
	void findSkinning() {
		Matrix34 temp;
		skinningMatrixes.clear();
		for (int i = 0; i < jointMatrixes.size(); i++) {
			temp = *jointMatrixes[i];
			temp.Dot(temp, invertedMatrixes[i]);
			skinningMatrixes.push_back(temp);
		}
	}
	void setMatrixes(std::vector<Matrix34*> m) { jointMatrixes = m; }
	skin(const char *f, std::vector<Matrix34*> m);
	void update();
	void draw();
	~skin();
	bool Load(const char *file);
};

#endif