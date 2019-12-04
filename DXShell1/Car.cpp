#include "Car.h"

bool Car::shouldDestroyPlayer = false;
bool Car::WeaponFired = false;



/*
Function: Car
Param:string newName, Graphics* gfx ( car name,  graphic handler)
Return: None
Des: Constructor
*/
 Car::Car(string newName, Graphics* gfx)
{

	 this->Name = newName;


	 if (this->Name == "Player")
	 {
		 this->Apperance = new SpriteSheet(L"playercar.bmp", gfx);
		 this->weapon = new Weapon(Car::VehicleMovementRate,"Player", gfx);
		 this->Destroyed = false;
	 }

	 if (this->Name == "Rival")
	 {
		 this->Apperance = new SpriteSheet(L"Rival.png", gfx);
		 this->weapon = new Weapon(Car::VehicleMovementRate, "Rival", gfx);
		 this->Destroyed = true;
	 }


	

	 this->explosion = new SpriteSheet(L"explosion.png", gfx);
	
	 //player initial position
	 if (this->Name == "Player")
	 {
		 this->CorX = Graphics::bottomX + 0.5* Graphics::roadWidth();
		 this->CorY = 6 * GRID_H;
	 }
	 

	 //rival initial position
	 if (this->Name == "Rival")
	 {
		 this->CorX = Graphics::bottomX + 0.25 * Graphics::roadWidth();
		 this->CorY = 10 * GRID_H;
	 }
	


}


/*
Function: GetName
Param:None
Return: String
Des: Getter
*/
 string Car::GetName()
 {
	 return this->Name;
}


/*
Function: GetCorX
Param:None
Return: float
Des: Getter
*/
 float Car::GetCorX()
 {
	 return this->CorX;
 }

/*
Function: SetCorX
Param: float newCarX
Return: None
Des: Setter
*/
 void Car::SetCorX(float newCarX)
 {
	 if (newCarX >= 0 && newCarX <= 10 * GRID_L)
	 {
		 this->CorX = newCarX;
	 }
 }

/*
Function: GetCorY
Param: None
Return: float
Des: Getter
*/
 float Car::GetCorY()
 {
	 return this->CorY;
 }

/*
Function: SetCorY
Param: float newCarY
Return: None
Des: Setter
*/
 void Car::SetCorY(float newCarY)
 {
	 if (newCarY >= 0 && newCarY <= 10 * GRID_H)
	 {
		 this->CorY = newCarY;
	 }
 }



/*
Function: GetCarShape
Param: None
Return:  SpriteSheet* 
Des: Getter
*/
 SpriteSheet* Car::GetCarShape()
 {
	 return this->Apperance;
 }

/*
Function: GetExplosionShape
Param: None
Return: SpriteSheet*
Des: Getter
*/
 SpriteSheet * Car::GetExplosionShape()
 {
	 return this->explosion;
 }

/*
Function: GetWeapon
Param: None
Return: Weapon*
Des: Constructor
*/
 Weapon* Car::GetWeapon()
 {
	 return this->weapon;
 }


/*
Function: Car
Param: none
Return: bool
Des: check if car is destroyed
*/
 bool Car::IsCarDestroyed()
 {
	 return this->Destroyed;
 }


/*
Function: SetCarDestroyed
Param:bool status
Return: None
Des: change car status
*/
 void Car::SetCarDestroyed(bool newStatus)
 {
	 this->Destroyed = newStatus;
 }




