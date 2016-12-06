/*
=================
cFish.h
- Header file for class definition - SPECIFICATION
- Header file for the Fish class which is a child of cSprite class
=================
*/
#ifndef _CFish_H
#define _CFish_H
#include "cSprite.h"


class cFish : public cSprite
{
private:
	SDL_Point FishVelocity;

public:
	cFish();
	void update(double deltaTime);		// Fish update method
	void setFishVelocity(SDL_Point FishVel);   // Sets the velocity for the Fish
	SDL_Point getFishVelocity();				 // Gets the Fish velocity
};
#endif