#pragma once
#include <string>
#include "Graphics.h"
#include "GameController.h"
#include "Weapon.h"
#include <time.h>



/*
Class: Car
Des: This class is the collection of the car attributes
*/
class Car 
{
private:
	string Name;
	SpriteSheet* Apperance;
	SpriteSheet* explosion;
	Weapon* weapon;
	bool Destroyed;

	//explosion sound


	float CorX;
	float CorY;

	
public:
   //status variabls
	const float topSpeed = 24;
	const float defaultSpeed = 8;
	static const int VehicleMovementRate = 5;
	static const int fireDelay = 1500; //fire a missle every 1500 ms
	static bool shouldDestroyPlayer;
	static bool WeaponFired;

   //getters and setters
	Car(string newName, Graphics* gfx);

	string GetName();

	float GetCorX();

	void SetCorX(float newCarX);

	float GetCorY();

	void SetCorY(float newCarY);

	SpriteSheet* GetCarShape();

	SpriteSheet* GetExplosionShape();

	Weapon* GetWeapon();

	bool IsCarDestroyed();

	void SetCarDestroyed(bool newStatus);


};
