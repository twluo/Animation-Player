#include "particle.h"


particle::particle()
{
	velocity = Vector3(0, 0, 0);
	position = Vector3(0, 0, 0);
	force = Vector3(0, 0, 0);
	mass = 1;
	Static = false;
}


particle::~particle()
{
}
