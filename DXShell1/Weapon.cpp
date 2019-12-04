#include "Weapon.h"




/*
Function:Weapon
Param:int carSpeed, string car, Graphics* gfx
Return: None
Des: Constructor
*/
Weapon::Weapon(int carSpeed, string car, Graphics* gfx)
{

	if (car == "Player")
	{
		this->weaponSheet = new SpriteSheet(L"trap.png", gfx);
	}

	if (car == "Rival")
	{
		this->weaponSheet = new SpriteSheet(L"Weapon.png", gfx);
	}


	if (car == "Player")
	{
		
		this->weaponSpeed =  carSpeed;

		if (carSpeed <= 0)
		{
			weaponSpeed = 2;
		}
	}

	if (car == "Rival")
	{
		this->weaponSpeed = 1.5 * carSpeed;

	}
	
	this->weaponFired = false;

	//weapon initial position
	this->WeaponX = XGridNum * GRID_L;
	this->WeaponY = YGridNum * GRID_H;

}


/*
Function: GetWeaponCorX
Param: None
Return: float
Des: Getter
*/
float Weapon::GetWeaponCorX()
{
	return this->WeaponX;
}

/*
Function: SetWeaponCorX
Param: float newCorX
Return: None
Des: Setter
*/
void Weapon::SetWeaponCorX(float newCorX)
{
	this->WeaponX = newCorX;
}

/*
Function: GetWeaponCorY
Param: none
Return: Float
Des: Getter
*/
float Weapon::GetWeaponCorY()
{
	return this->WeaponY;
}

/*
Function: SetWeaponCorY
Param: float newCorY
Return: None
Des: Setter
*/
void Weapon::SetWeaponCorY(float newCorY)
{
	this->WeaponY = newCorY;
}

/*
Function: GetWeaponSpeed
Param: None
Return: float 
Des: Getter
*/
float Weapon::GetWeaponSpeed()
{
	return this->weaponSpeed;
}

/*
Function: IsWeaponFired
Param: None
Return: Bool
Des: check if weapon is fired
*/
bool Weapon::IsWeaponFired()
{
	return this->weaponFired;
}

/*
Function: SetWeaponFired
Param: bool newStatus
Return: None
Des: Setter
*/
void Weapon::SetWeaponFired(bool newStatus)
{
	this->weaponFired = newStatus;
}


/*
Function: ReSetWeaponPos
Param: None
Return: none
Des: Reset weapon position
*/
void Weapon::ReSetWeaponPos()
{
	//weapon initial position
	this->WeaponX = XGridNum * GRID_L;
	this->WeaponY = YGridNum * GRID_H;
}


/*
Function: GetWeaponShape
Param: None
Return: SpriteSheet * 
Des: Getter
*/
SpriteSheet * Weapon::GetWeaponShape()
{
	return this->weaponSheet;
}


