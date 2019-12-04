#pragma once


#include "Graphics.h"
#include "GameController.h"



/*
class: Weapon
Des: This class is the collection of the weapon as part of the car class
*/
class Weapon
{

	float WeaponX;
	float WeaponY;
	float weaponSpeed; //set the missle projectile speed 

	bool weaponFired;

	SpriteSheet* weaponSheet;

	//fire sound


public:



	Weapon(int Speed, string car, Graphics* gfx);

	float GetWeaponCorX();
	void SetWeaponCorX(float  newCorX);
	float GetWeaponCorY();
	void SetWeaponCorY(float newCorY);
	float GetWeaponSpeed();
	bool IsWeaponFired();
	void SetWeaponFired(bool newStatus);

	void ReSetWeaponPos();

	SpriteSheet* GetWeaponShape();


};

