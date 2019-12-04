/*
Project: SET Hunter
File: Level 1.h
Programmer: Shuang Liang 7492259
Description: This is class definition for level 1 logics
*/


#pragma once
#include "GameLevel.h"
#include"Graphics.h"
#include "Car.h"
#include "Plant.h"
#include "Timer.h"
#include <vector>
#include "soundclass.h"

using namespace std;

 


/*
Class: Level1
Des:  This is the all game logic in the level 1
*/
class Level1 : public GameLevel
{

	Car* PlayerCar;
	Car* Rival;



	Plant* tree1;
	Plant* tree2;
	Plant* shrub;
	vector<Plant*> collections;

	Timer* gameTimer;
	Timer* punishTimer;
	Timer* rivalTimer;


	// Variable for creating the grids of plants
	int currentGrid[XGridNum][YGridNum];
	int bufferGrid[XGridNum][YGridNum];
	static bool shouldCreateNewBuffer;
	static bool CarOnRoad;

public:
	
	//game static parameters for the game logic 
	static int LifeRemained;
	static unsigned long currentScores;
	static unsigned long finalScores;
	static bool endOfGame;
    
	//sound object
	static SoundClass* Sound;

    
	//override functions
	void Load() override;
	void Unload() override;
	void Update() override;
	void Render() override;

	//pass the key control from the keyboard
	void Control(int x, int y) override;
  

  //player control for trap
	void SetTrap() override;


	//update plant buffer
	 void UpdateBufferStatus();
	 
	 //move the plants
	 void UpdatePlantLocation();

	 //control the car
	 void MoveCarFromControl(int x, int );

	 //rival follow player
	 void UpdateRivalPlayerRelation();

	 //rival fire missle on player
	 void UpdateWeaponTrapPos();




	 //check whether player is on the road
	 void CheckCarRoadPos();


	 //check if player crashes into the plants
	 void CheckCollision(int a, int b);

	 //check weapon effectiveness
	 void CheckWeaponEffectiveness();

	 //reset all object locations
	 void ResetObjects();

	 //reset the score and life
	 void ResetScore();

	 //cereate new random plants
	 void CreateNewPlants();

	 //draw all the plants in the grid
	 void DrawPlants();


	 enum plantType { Tree1, Tree2, Shrub, Empty };




};
