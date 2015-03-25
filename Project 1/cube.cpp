////////////////////////////////////////
// cube.cpp
////////////////////////////////////////

#include "cube.h"
#include <iostream>
#include "Skeleton.h"

////////////////////////////////////////////////////////////////////////////////

SpinningCube::SpinningCube() {
	// Initialize constant data
	//Size=1.0f;
	//Position=Vector3::ORIGIN;
	//Axis=Vector3::YAXIS;

	// Resets variable data
	Reset();
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Update() {
	// Update (animate) any variable properties
	Angle+=0.005f;
	WorldMtx.MakeRotateUnitAxis(Axis,Angle);
	WorldMtx.d=Position;
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Reset() {
	// Reset dynamic variables to a default state
	Angle=0.0f;
	WorldMtx.Identity();
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Draw() {
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(WorldMtx);
	//glutWireCube(Size);
	/*
	Joint test = Joint("Test");
	test.setPose(Vector3(45, 45, 0));
	test.setTranslation(Vector3(0, 0, 0));
	test.setScale(Vector3(1, 1, 1), Vector3(0, 0, 0));
	test.calculate();
	Joint test1 = Joint("Test2");
	test1.setDOFX(0);
	test1.setDOFY(0);
	test1.setDOFZ(0);
	test1.setTranslation(Vector3(0, 1.5, 0));
	test1.setScale(Vector3(1, 1, 1), Vector3(0, 0, 0));
	test1.calculate();
	test.addChild(test1);
	Joint *test2 = test1.getParent();
	test.draw(WorldMtx.IDENTITY);*/

	Skeleton test = Skeleton();
	test.Load("test.skel");
	test.calculate(WorldMtx.IDENTITY);
	test.draw();
}

////////////////////////////////////////////////////////////////////////////////
