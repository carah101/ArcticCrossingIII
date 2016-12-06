/*
== == == == == == == == =
cSeal.h
- Header file for class definition - SPECIFICATION
- Header file for the Seal class which is a child of cSprite class
== == == == == == == == =
*/
#ifndef _CSEAL_H
#define _CSEAL_H
#include "cSprite.h"

class cSeal : public cSprite
{
private:
	SDL_Point sealMotion;

	
public:
	cSeal();
	void initialise();
	//void render();		// Default render function
	void update(float deltaTime);		// Seal update method
	void setSealMotion(SDL_Point sealMotion);   //sets the distance moved by the seal
	SDL_Point getSealMotion();				 //gets the distance moved by the seal
};
#endif