/*
=================
cIce.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cIce.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cIce::cIce() : cSprite()
{
	this->IceVelocity = { 0.0, 0.0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cIce::update(double deltaTime)
{

	/*this->setSpriteRotAngle(this->getSpriteRotAngle() +(0.0f * deltaTime)); 
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() -360);
	}*/

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime *2 ;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime *2 ;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	//cout << "Ice position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Ice
=================================================================
*/
void cIce::setIceVelocity(SDL_Point IceVel)
{
	IceVelocity = IceVel;
}
/*
=================================================================
  Gets the Ice velocity
=================================================================
*/
SDL_Point cIce::getIceVelocity()
{
	return IceVelocity;
}
