/*
Project: SET Hunter
File: Level1.cpp
Programmer: Shuang Liang 7492259
Description: This si the implementation of the level 1 logic for plants and cars
*/


#include "GameController.h"
#include "Level1.h"
#include "mmsystem.h"
#include <Windows.h>
#include <time.h>
#pragma comment (lib,"winmm.lib")


//initialize all static class memebers
int Plant::treeAddY = 0;
float Plant::acceleration = 0;
bool Level1::shouldCreateNewBuffer = false;
int Level1::LifeRemained = 3;
unsigned long Level1::currentScores = 0;
unsigned long Level1::finalScores = 0;
bool Level1::CarOnRoad = true;
bool Level1::endOfGame = false;

/*
Fucntion: Load
Parameter: None 
Return: None 
Description: Constructor'-
*/


//creating a static class member for playing sound
 SoundClass* Level1::Sound = new SoundClass();


/*
Function: load
Param: None
Return: None
Des: This is the loading for the level 1 objects
*/
void Level1::Load() 
{

	gameTimer = new Timer();
	punishTimer = new Timer();
	rivalTimer = new Timer();

	this->PlayerCar = new Car("Player", gfx);
	this->Rival = new Car("Rival", gfx);

	this->tree1 = new Plant("tree1", gfx);
	this->tree2 = new Plant("tree2", gfx);
	this->shrub = new Plant("shrub", gfx);	
	

	//has to be this sequence
	collections.push_back(tree1);
	collections.push_back(tree2);
	collections.push_back(shrub);


	for (int i = 0; i < XGridNum; i++)
	{
		for (int j = 0; j < YGridNum; j++)
		{
			currentGrid[i][j] = Empty;
		}
	}


	CreateNewPlants();

}

/*
Function: 
Param: 
Return:
Des:
*/
void Level1::Unload()
{
	delete PlayerCar;
	delete tree1;
	delete tree2;
	delete shrub;
	delete gameTimer;
	delete punishTimer;
	delete rivalTimer;

	collections.clear();

}




/*
Fucntion: Update
Parameter: None
Return: None
Description: Update background and obejct positions
*/
void Level1::Update()
{ 




	//update should create buffer status
	UpdateBufferStatus();

	//update road 
	gfx->UpdateBezierPoints(Graphics::currentDirection, Plant::treeAddY);



	//update plants position
	UpdatePlantLocation();


	//rival check player position 
	UpdateRivalPlayerRelation();



	//update the weapon position
	UpdateWeaponTrapPos();


	//check if the car is on the road
	CheckCarRoadPos();

	//update score
	currentScores = gameTimer->GetDuration();


}



/*
Fucntion: Render
Parameter: None
Return: None 
Description: Draw background and objects 
*/
void Level1::Render()
{

	//mciSendString("end background", NULL, 0, NULL);
	//mciSendString("open background.wav type waveaudio alias background", NULL, 0, NULL);
	//mciSendString("play background", NULL, 0, NULL);




	//first have two methods froms graphic layer
	//clear screen with green background
	gfx->ClearScreen(0.0f, 0.5f, 0.0f);


	// then have two at the level 1 layer
	//draw plants both current and buffer
	DrawPlants();


	gfx->DrawRoad(Graphics::roadAddY);



	if (!Rival->IsCarDestroyed())
	{
		//draw rival
		Rival->GetCarShape()->Draw(Rival->GetCorX(), Rival->GetCorY());



		if (Car::shouldDestroyPlayer)
		{
			//draw missile
			Rival->GetWeapon()->GetWeaponShape()->Draw(Rival->GetWeapon()->GetWeaponCorX(), Rival->GetWeapon()->GetWeaponCorY());
		}
	}


	//draw player
	PlayerCar->GetCarShape()->Draw(PlayerCar->GetCorX(),PlayerCar->GetCorY());

	//draw the trap
	PlayerCar->GetWeapon()->GetWeaponShape()->Draw(PlayerCar->GetWeapon()->GetWeaponCorX(), PlayerCar->GetWeapon()->GetWeaponCorY());



	//draw the scores and life
	gfx->WriteText(currentScores, LifeRemained);

}


/*
Fucntion: DrawPlants
Parameter: None 
Return: None 
Description: Draw plants implementation
*/
void Level1::DrawPlants()
{
	
		//draw the plants in current grid
		for (int a = 0; a < XGridNum; a++)
		{
			for (int b = 0; b < YGridNum; b++)
			{
				if (currentGrid[a][b] != Empty)
				{

				
					CheckCollision(a, b);

					collections[currentGrid[a][b]]->GetPlantSheet()->Draw(a*GRID_L, b*GRID_H + Plant::treeAddY);
	
			    }

		
				
			}
		}

		//draw the plants in buffer grid
		for (int a = 0; a < XGridNum; a++)
		{
			for (int b = 0; b < YGridNum; b++)
			{
				if (bufferGrid[a][b] != Empty)
				{
					collections[bufferGrid[a][b]]->GetPlantSheet()->Draw(a*GRID_L, b*GRID_H - GRID_H*YGridNum + Plant::treeAddY);

				}

			}
		}

}


/*
Fucntion: CheckCollision
Parameter: a , b the grid position 
Return: None 
Description: check grid plant and car collision
*/
void Level1::CheckCollision(int a, int b)
{
	
	float CarPositionX = PlayerCar->GetCorX();
	float CarPositionY = PlayerCar->GetCorY();
	float  BottomLeftEdge = Graphics::bottomX + Graphics::roadMovement - 0.5*GRID_L;
	float BottomRightEdge = Graphics::bottomX + Graphics::roadMovement + Graphics::roadWidth() + 0.5*GRID_L;

	// if the car is not on the road
	if (CarPositionX <= BottomLeftEdge || CarPositionX >= BottomRightEdge)
	{
		if (CarPositionX >= (a - 0.4) * GRID_L && CarPositionX <= (a + 0.6) * GRID_L  && CarPositionY >= (b - 0.4) * GRID_H + Plant::treeAddY && CarPositionY <= (b + 0.6) * GRID_H + Plant::treeAddY)
		{

			PlayerCar->GetExplosionShape()->Draw(a*GRID_L, b*GRID_H + Plant::treeAddY);

			Sound->PlayWaveFile(false, SoundClass::SoundColletion::Explosion);

			//timer,life score reset
			ResetScore();

			Sleep(300);

			//reset all objects status
			ResetObjects();

		}
	}
}

/*
Fucntion: CreateNewPlants
Parameter: None 
Return: None 
Description: Createa a buffer of new random plants
*/ 
void Level1::CreateNewPlants()
{

	time_t seed;
	srand((unsigned)time(&seed));


	int randNum;

	for (int i = 0; i < XGridNum; i++)
		{
			for (int j = 0; j < YGridNum; j++)
			{
			/*	if (i >= XGridNum * 0.4 && i <= YGridNum * 0.6) 
				{
					bufferGrid[i][j] = Empty; continue;
				}
				*/

				randNum = rand() % 60;

				switch (randNum) {
				case Tree1:
					
					bufferGrid[i][j] = Tree1;

					break;
				case Tree2:
					
					bufferGrid[i][j] = Tree2;
					break;
				case Shrub:
					
					bufferGrid[i][j] = Shrub;
					break;

				default:

					bufferGrid[i][j] = Empty;
				}

			}


	}
}


/*
Fucntion: UpdatePlantLocation
Parameter: None 
Return: None
Description: Update the plant location according to the addY
*/
void Level1::UpdatePlantLocation()
{   


	//prevent from backward
	if (Plant::treeAddY <= 0)
	{
		Plant::treeAddY = 0;
		if (Plant::acceleration <= 0)
		{
			Plant::acceleration = 0;
		}
	}


	Plant::treeAddY += Plant::acceleration;

	//control the top speed
	if (Plant::acceleration >= Plant::topSpeed() - Plant::defaultSpeed())
	{
		Plant::acceleration = Plant::topSpeed() - Plant::defaultSpeed();
	}


	if (shouldCreateNewBuffer)
	{
		//copy buffer to current
		for (int i = 0; i < XGridNum; i++)
		{
			for (int j = 0; j < YGridNum; j++)
			{
				currentGrid[i][j] = bufferGrid[i][j];
			}


		}

		//create new random locations for buffer grid
		CreateNewPlants();

		shouldCreateNewBuffer = false;

	}
	else
	{  
		// increment addY so the tree will move doward and keep raod moves at the same pace with plant
		Plant::treeAddY += Plant::acceleration;
		Graphics::roadAddY += Plant::acceleration;

	}


}



/*
Function: MoveCarFromControl
Param: x, y player positon
Return: None
Des:  This function controls the player car based on the kyeboard feedback
*/
void Level1::MoveCarFromControl(int x, int y)
{


	//  Move car location
	PlayerCar->SetCorX(PlayerCar->GetCorX() + x);
	PlayerCar->SetCorY(PlayerCar->GetCorY() + y);


	//restrict player to be within certain area of the screen
	if (PlayerCar->GetCorY() <= 0.2 * YGridNum * GRID_H)
	{
		PlayerCar->SetCorY(0.2 * YGridNum * GRID_H);
	}

	if (PlayerCar->GetCorY() >= 0.8 * YGridNum * GRID_H)
	{
		PlayerCar->SetCorY(0.8 * YGridNum * GRID_H);
	}

	

}







/*
Function: UpdateRivalPlayerRelation
Param:  None
Return: None
Des: This function 
*/

//rival check player position, if on the road then delay one second to follow the x coordinate and fire the weapon,
//if the player is not on the road, then keep moving along the road.


void Level1::UpdateRivalPlayerRelation()
{

	float PlayerXCor = PlayerCar->GetCorX();


	float RivalXcor = Rival->GetCorX();
	
	float RivalYcor = Rival->GetCorY();



	// if the game is started
	if (gameTimer->IsStarted())
	{

		//reset rival if destroyed
		if (Rival->IsCarDestroyed())
		{
		

			Rival->SetCorX(Graphics::bottomX + 0.25 * Graphics::roadWidth());
			Rival->SetCorY(10 * GRID_H);
			rivalTimer->Start();

		}


		long currentDuration = rivalTimer->GetDuration();

		//rival appears after 5000 ms of the game time
		if (currentDuration >= 5000 && Rival->IsCarDestroyed())
		{
	
			Rival->SetCarDestroyed(false);

			Sound->LoadWaveFile("enemy-converted.wav", SoundClass::SoundColletion::Enemy);
			Sound->ShutdownWaveFile(SoundClass::SoundColletion::Background);
			Sound->PlayWaveFile(true, SoundClass::SoundColletion::Enemy);

		}

		//move to rival position
		if (rivalTimer->IsStarted() && !Rival->IsCarDestroyed())
		{
			Rival->SetCorY(Rival->GetCorY() - Car::VehicleMovementRate);

			// move to the bottom of the road
			if (Rival->GetCorY() <= 0.8 * YGridNum* GRID_H)
			{
				Rival->SetCorY(0.8 * YGridNum* GRID_H);
			}



			//current play is on the road follow the player car once reach fire the missle  but horizontal movement speed is only 30% of the player
			if (CarOnRoad)
			{
				//player is on the right side of rival
				if (PlayerXCor > RivalXcor)
				{
					Rival->SetCorX(Rival->GetCorX() + 0.3 * Car::VehicleMovementRate);
				}

				//player is on the left side of the rival
				if (PlayerXCor < RivalXcor)
				{
					Rival->SetCorX(Rival->GetCorX() - 0.3 * Car::VehicleMovementRate);
				}

				//if reach playerX fire the weapon
				Car::shouldDestroyPlayer = true;

			}

			//stick to the terrain of the road
			else
			{
				float centerOfRoad = Graphics::bottomX + 0.5 *  Graphics::roadWidth();


				if (RivalXcor > centerOfRoad)
				{
					Rival->SetCorX(Rival->GetCorX() - 0.5 * Car::VehicleMovementRate);
				}
				if (RivalXcor < centerOfRoad)
				{
					Rival->SetCorX(Rival->GetCorX() + 0.5 * Car::VehicleMovementRate);
				}

				Car::shouldDestroyPlayer = false;
			}



		}




	
	}
	



}




/*
Function: UpdateWeaponTrapPos
Param: None
Return: None
Des: update the weapons positions
*/
void Level1::UpdateWeaponTrapPos()
{

	  //creat the inital position of the missle at the position of the rival if needs to fire
	

	if (rivalTimer->IsStarted() && !Rival->IsCarDestroyed())
	{
		if (Car::shouldDestroyPlayer && !Rival->GetWeapon()->IsWeaponFired())
		{

			Rival->GetWeapon()->SetWeaponFired(true);

			Rival->GetWeapon()->SetWeaponCorX(Rival->GetCorX());
			Rival->GetWeapon()->SetWeaponCorY(Rival->GetCorY());

		}

		if (Rival->GetWeapon()->IsWeaponFired())
		{
			//then update it based on the intial position if the weapon is in the screen
			//if (Rival->GetWeapon()->GetWeaponCorY() < 0.8 * GRID_H *YGridNum && Rival->GetWeapon()->GetWeaponCorX() < GRID_H * YGridNum)

			Rival->GetWeapon()->SetWeaponCorY(Rival->GetWeapon()->GetWeaponCorY() - Rival->GetWeapon()->GetWeaponSpeed());

	



			//if reach top of the screen back to current rival position
			if (Rival->GetWeapon()->GetWeaponCorY() < 0)
			{
				if (Car::shouldDestroyPlayer)
				{
					Rival->GetWeapon()->SetWeaponCorX(Rival->GetCorX());
					Rival->GetWeapon()->SetWeaponCorY(Rival->GetCorY());
				}
				else

				{
					Rival->GetWeapon()->ReSetWeaponPos();
				}


			}
		}
	}



	//now update the trap position
	if (PlayerCar->GetWeapon()->IsWeaponFired())
	{
		PlayerCar->GetWeapon()->SetWeaponCorY(PlayerCar->GetWeapon()->GetWeaponCorY() + PlayerCar->GetWeapon()->GetWeaponSpeed());


		//out of screen reset to inital location and reset fire status
		if (PlayerCar->GetWeapon()->GetWeaponCorY() >  YGridNum* GRID_H)
		{
			PlayerCar->GetWeapon()->ReSetWeaponPos();
			PlayerCar->GetWeapon()->SetWeaponFired(false);
		}



	}


	//check collision of the weapon and trap

	CheckWeaponEffectiveness();




}




/*
Function: CheckWeaponEffectiveness
Param: None
Return: None
Des: Check if weapon hits the player or the rival
*/
void Level1::CheckWeaponEffectiveness()
{


	//if the player got hit by weapon reset all
	float playerX = PlayerCar->GetCorX();
	float playerY = PlayerCar->GetCorY();

	float weaponX = Rival->GetWeapon()->GetWeaponCorX();
	float weaponY = Rival->GetWeapon()->GetWeaponCorY();


	float offset = 0.2;

	if ((playerX >= weaponX - offset * GRID_L && playerX <= weaponX + offset * GRID_L) && (playerY >= weaponY - offset * GRID_H && playerY <= weaponY + offset * GRID_H))
	{

		PlayerCar->GetExplosionShape()->Draw(PlayerCar->GetCorX(), PlayerCar->GetCorY());

		Sound->PlayWaveFile(false, SoundClass::SoundColletion::Explosion);

		ResetScore();

		Sleep(1000);

		ResetObjects();
	}


	//if the rival got hit by trap wait for 5 seconds and reset the rival
	if (rivalTimer->IsStarted())
	{
		//get trap position and rival position
		float trapX = PlayerCar->GetWeapon()->GetWeaponCorX();
		float trapY = PlayerCar->GetWeapon()->GetWeaponCorY();

		float rivalX = Rival->GetCorX();
		float rivalY = Rival->GetCorY();
		

		if ((rivalX >= trapX - offset * GRID_L && rivalX <= trapX + offset * GRID_L) && (rivalY >= trapY - offset * GRID_H && rivalY <= trapY + offset * GRID_H))
		{
			Rival->GetExplosionShape()->Draw(Rival->GetCorX(), Rival->GetCorY());

			Sound->PlayWaveFile(false, SoundClass::SoundColletion::Explosion);

			Rival->SetCarDestroyed(true);

			rivalTimer->Stop();
			rivalTimer->Reset();
		
		}
	}



}




/*
Function: CheckCarRoadPos
Param: None
Return: None
Des: check if the car is on the road
*/
void Level1::CheckCarRoadPos()
{
	//check if the car is on the road 
	float offset = 0.1;
	float CarPosition = PlayerCar->GetCorX();
	float  BottomLeftEdge = Graphics::bottomX + Graphics::roadMovement - offset *GRID_L;
	float BottomRightEdge = Graphics::bottomX + Graphics::roadMovement + Graphics::roadWidth() + offset *GRID_L;

	//right now on the road
	if (CarPosition >= BottomLeftEdge && CarPosition <= BottomRightEdge)
	{ 
		//cross the border to be back on road need to pubish 3000ms
		if (!CarOnRoad)
		{
			
			punishTimer->Start();

			if (punishTimer->GetDuration() >= 3000)
			{
				CarOnRoad = true;
				gameTimer->Resume();
				punishTimer->Stop();
				punishTimer->Reset();
	
			}
		}

		
	}

	//right now off the road
	else
	{
		//the car cross the border to be off the road
		if (CarOnRoad)
		{
			CarOnRoad = false;
			gameTimer->Pause();
		}

	}
}



/*
Function: ResetObjects
Param: None
Return: None
Des: reset all object position
*/
void Level1::ResetObjects()
{
	//reset car location
	PlayerCar->SetCorX(Graphics::bottomX + 0.5* Graphics::roadWidth());
	PlayerCar->SetCorY(6 * GRID_H);

	//reset rival location
	Rival->SetCorX(Graphics::bottomX + 0.25 * Graphics::roadWidth());
	Rival->SetCorY(10 * GRID_H);


	//reset trap position
	PlayerCar->GetWeapon()->SetWeaponCorX(10 * GRID_L);
	PlayerCar->GetWeapon()->SetWeaponCorY(10 * GRID_H);
	PlayerCar->GetWeapon()->SetWeaponFired(false);


	//reset weapon position
	Rival->GetWeapon()->SetWeaponCorX(10 * GRID_L);
	Rival->GetWeapon()->SetWeaponCorY(10 * GRID_H);
	Rival->GetWeapon()->SetWeaponFired(false);
	Rival->SetCarDestroyed(true);

	//reset destroy option
	Car::shouldDestroyPlayer = false;


	//reset tree movement and acceleration
	Plant::treeAddY = 0;
	Plant::acceleration = 0;


	//reset rival timer
	rivalTimer->Stop();
	rivalTimer->Reset();


}



/*
Function: ResetScore
Param: None
Return: None
Des: reset the score and timers
*/
void Level1::ResetScore()
{

	//reset life and score and deduce a life
	if (LifeRemained > 1)
	{
		LifeRemained -= 1;
		currentScores = 0;
		gameTimer->Stop();
		gameTimer->Reset();

	}

	//the end of game reset all
	else
	{
		LifeRemained = 3;
		finalScores = currentScores;
		currentScores = 0;
		gameTimer->Stop();
		gameTimer->Reset();

		endOfGame = true;

	}

	//always reset punishTimer
	punishTimer->Stop();
	punishTimer->Reset();
	

	//reset background music
	Sound->ShutdownWaveFile(SoundClass::SoundColletion::Enemy);
	Sound->LoadWaveFile("background-converted.wav", SoundClass::SoundColletion::Background);
	Sound->PlayWaveFile(true, SoundClass::SoundColletion::Background);
}




/*
Fucntion: UpdateBufferStatus
Parameter: None
Return: None
Description: reset the buffer
*/
void Level1::UpdateBufferStatus()
{


	if (Plant::treeAddY >= YGridNum * GRID_H)
	{
		shouldCreateNewBuffer = true;
		Plant::treeAddY = 0;
		Graphics::currentDirection = gfx->CrateNewDirection();
	}

}


/*
Fucntion: Control
Parameter: int x, int y
Return: None
Description: Pass the keyboard control to car movement and acceleration
*/
void Level1::Control(int x, int y)
{
		
	//change to car location from the key
	//When the car is stop can not move left or right
	if (Plant::acceleration == 0)
	{
		x = 0;
	}

	MoveCarFromControl(x, y);

	//add to plant acceleration
	//only when the car move forward and on the road do we start the timer
	if (y < 0)
	{

		Plant::acceleration += 1;



		if (PlayerCar->GetCorY() <= 0.2 * YGridNum* GRID_H)
		{
			Sound->PlayWaveFile(false, SoundClass::SoundColletion::Accelerate);
		}
	
			gameTimer->Start();
		
		
	}

	if (y > 0)
	{

		Plant::acceleration -= 0.5;
	}

	


}







/*
Function: SetTrap
Param: None
Return: None
Des: User press space to set a mine 
*/
void Level1::SetTrap()
{
	
	//set the mine to be at current position of the player

	if (!PlayerCar->GetWeapon()->IsWeaponFired())
	{
		PlayerCar->GetWeapon()->SetWeaponCorX(PlayerCar->GetCorX());
		PlayerCar->GetWeapon()->SetWeaponCorY(PlayerCar->GetCorY());
		PlayerCar->GetWeapon()->SetWeaponFired(true);

		Sound->PlayWaveFile(false, SoundClass::SoundColletion::Weapon);
	}
	

}
