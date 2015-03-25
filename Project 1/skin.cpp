#include "skin.h"
#include <iostream>


skin::skin()
{
	Load("tube_smooth.skin");
	findInverses();
	findSkinning();
	std::cout << "position size: " << positions.size() << " normals size: " << normals.size() << " skinwieghts size: " << skinWeights.size() << " triangles size: " << triangles.size() << " bindings size: " << matrixes.size() << std::endl;
}

skin::skin(const char* f, std::vector<Matrix34*> m)
{
	Load(f); 
	jointMatrixes = m;
	findInverses();
	findSkinning();
}
skin::~skin()
{
}
void skin::update() {
	Matrix34 mtemp;
	Vector3 ptemp;
	Vector3 ntemp;
	blendedPositions.clear();
	blendedNormals.clear();
	findSkinning();
	for (int i = 0; i < positions.size(); i++){
		Vector3 pos = Vector3(0, 0, 0);
		Vector3 nor = Vector3(0, 0, 0);
		for (int j = 0; j < skinWeights[i].size(); j++) {
			mtemp = skinningMatrixes[skinWeights[i][j].getIndex()];
			mtemp.Transform(positions[i], ptemp);
			mtemp.Transform3x3(normals[i], ntemp);
			ptemp = ptemp * skinWeights[i][j].getWeight();
			ntemp = ntemp * skinWeights[i][j].getWeight();
			pos = pos + ptemp;
			nor = nor + ptemp;
		}
		blendedPositions.push_back(pos);
		nor.Normalize();
		blendedNormals.push_back(nor);
	}
}

void skin::draw() {
	glMatrixMode(GL_MODELVIEW);
	Matrix34 id;
	glLoadMatrixf(id.IDENTITY);
	Vector3 temp;
	Vector3 tempVertex;
	Vector3 tempNorm;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < triangles.size(); i++) {
		temp = triangles[i];
		tempVertex = blendedPositions[temp.x];
		tempNorm = blendedNormals[temp.x];
		glNormal3f(tempNorm.x, tempNorm.y, tempNorm.z);
		glVertex3f(tempVertex.x, tempVertex.y, tempVertex.z);
		tempVertex = blendedPositions[temp.y];
		tempNorm = blendedNormals[temp.y];
		glNormal3f(tempNorm.x, tempNorm.y, tempNorm.z);
		glVertex3f(tempVertex.x, tempVertex.y, tempVertex.z);
		tempVertex = blendedPositions[temp.z];
		tempNorm = blendedNormals[temp.z];
		glNormal3f(tempNorm.x, tempNorm.y, tempNorm.z);
		glVertex3f(tempVertex.x, tempVertex.y, tempVertex.z);
	}
	glEnd();
}
bool skin::Load(const char *file) {
	Tokenizer token;
	token.Open(file);
	// Parse tree
	Vector3 vtemp;
	int num;
	int switchCase;
	float ftemp;
	int itemp;
	positions.clear();
	normals.clear();
	skinWeights.clear();
	triangles.clear();
	matrixes.clear();
	while (1) {
		char temp[256];
		token.GetToken(temp);
		if (strcmp(temp, "positions") == 0) {
			switchCase = 0;
			num = token.GetInt();
			token.SkipLine();
		}
		else if (strcmp(temp, "normals") == 0) {
			switchCase = 1;
			num = token.GetInt();
			token.SkipLine();
		}
		else if (strcmp(temp, "skinweights") == 0) {
			switchCase = 2;
			num = token.GetInt();
			token.SkipLine();
		}
		else if (strcmp(temp, "triangles") == 0) {
			switchCase = 3;
			num = token.GetInt();
			token.SkipLine();
		}
		else if (strcmp(temp, "bindings") == 0) {
			switchCase = 4;
			num = token.GetInt();
			token.SkipLine();
		}
		else {
			token.SkipLine();
			switchCase = 5;
		}// Unrecognized token
		skinWeight sktemp;
		std::vector<skinWeight> skvtemp;
		int altNum;
		Matrix34 mtemp;
		for (int i = 0; i < num; i++) {
			switch (switchCase) {
			case 0:
				vtemp.x = token.GetFloat();
				vtemp.y = token.GetFloat();
				vtemp.z = token.GetFloat();
				positions.push_back(vtemp);
				break;
			case 1:
				vtemp.x = token.GetFloat();
				vtemp.y = token.GetFloat();
				vtemp.z = token.GetFloat();
				normals.push_back(vtemp);
				break;
			case 2:
				altNum = token.GetFloat();
				skvtemp.clear();
				for (int j = 0; j < altNum; j++) {
					itemp = token.GetFloat();
					ftemp = token.GetFloat();
					sktemp.setIndex(itemp);
					sktemp.setWeight(ftemp);
					skvtemp.push_back(sktemp);
				}
				skinWeights.push_back(skvtemp);
				break;
			case 3:
				vtemp.x = token.GetFloat();
				vtemp.y = token.GetFloat();
				vtemp.z = token.GetFloat();
				triangles.push_back(vtemp);
				break;
			case 4:token.SkipLine();
				vtemp.x = token.GetFloat();
				vtemp.y = token.GetFloat();
				vtemp.z = token.GetFloat();
				mtemp.a = vtemp; token.SkipLine();
				vtemp.x = token.GetFloat();
				vtemp.y = token.GetFloat();
				vtemp.z = token.GetFloat();
				mtemp.b = vtemp; token.SkipLine();
				vtemp.x = token.GetFloat();
				vtemp.y = token.GetFloat();
				vtemp.z = token.GetFloat();
				mtemp.c = vtemp; token.SkipLine();
				vtemp.x = token.GetFloat();
				vtemp.y = token.GetFloat();
				vtemp.z = token.GetFloat();
				mtemp.d = vtemp; token.SkipLine();token.SkipLine();
				matrixes.push_back(mtemp);
				break;
			}
		}
		if (switchCase == 4)
			break;
	}
	// Finish
	token.Close(); 
	return true;
}
