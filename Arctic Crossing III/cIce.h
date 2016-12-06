/*
=================
cIce.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CIce_H
#define _CIce_H
#include "cSprite.h"

class cIce : public cSprite
{
private:
	SDL_Point IceVelocity;

public:
	cIce();
	void update(double deltatime);
	void setIceVelocity(SDL_Point IceVel);   // Sets the velocity for the Ice
	SDL_Point getIceVelocity();				 // Gets the Ice velocity
};
#endif