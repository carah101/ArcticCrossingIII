/*
=================
cFish.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cFish.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cFish::cFish() : cSprite()
{
	this->FishVelocity = { 0.0, 0.0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cFish::update(double deltaTime)
{

	FPoint direction = { 0.0, 0.0};
	direction.X = (sin((this->getSpriteRotAngle())*PI/180));
	direction.Y = -(cos((this->getSpriteRotAngle())*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->FishVelocity.x = this->FishVelocity.x + direction.X;
	this->FishVelocity.y = this->FishVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->FishVelocity.x * deltaTime;
	currentSpritePos.y += this->FishVelocity.y * deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });

	//cout << "Fish position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;

	this->setBoundingRect(this->getSpritePos());

}
/*
=================================================================
  Sets the velocity for the Fish
=================================================================
*/
void cFish::setFishVelocity(SDL_Point FishVel)
{
	this->FishVelocity = FishVel;
}
/*
=================================================================
  Gets the Fish velocity
=================================================================
*/
SDL_Point cFish::getFishVelocity()
{
	return this->FishVelocity;
}
