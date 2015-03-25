////////////////////////////////////////
// cube.h
////////////////////////////////////////

#ifndef CSE169_CUBE_H
#define CSE169_CUBE_H

#include "core.h"
#include "vector3.h"
#include "matrix34.h"

////////////////////////////////////////////////////////////////////////////////

class SpinningCube {
public:
	SpinningCube();

	void Update();
	void Reset();
	void Draw();

private:
	// Constants
	float Size;
	Vector3 Position;
	Vector3 Axis;

	// Variables
	float Angle;
	Matrix34 WorldMtx;
};

////////////////////////////////////////////////////////////////////////////////

/*
SpinningCube is an example of a basic animating object. It can be used as a
pattern for creating more complex objects.
*/

#endif
