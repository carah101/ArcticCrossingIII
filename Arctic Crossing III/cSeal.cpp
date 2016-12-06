/*
=================
cSeal.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cSeal.h"
/*
=================================================================
Defualt Constructor
=================================================================
*/
cSeal::cSeal() : cSprite()
{
	this->sealMotion = { 0.0f, 0.0f };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cSeal::update(float deltaTime)
{
	FPoint direction = { 0.0f, 0.0f };
	direction.X = (sin(this->getSpriteRotAngle()*PI / 180));
	direction.Y = -(cos(this->getSpriteRotAngle()*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->sealMotion.x = this->sealMotion.x + direction.X;
	this->sealMotion.y = this->sealMotion.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->sealMotion.x * deltaTime;
	currentSpritePos.y += this->sealMotion.y * deltaTime;

	this->sealMotion.x *= 0.95;
	this->sealMotion.y *= 0.95;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
Sets the velocity for the rocket
=================================================================
*/
void cSeal::setSealMotion(SDL_Point rocketVel)
{
	sealMotion = rocketVel;
}
/*
=================================================================
Gets the rocket velocity
=================================================================
*/
SDL_Point cSeal::getSealMotion()
{
	return sealMotion;
}
