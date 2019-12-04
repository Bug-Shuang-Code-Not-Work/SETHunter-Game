/*
Project: SET Hunter
File: GameController.cpp
Programmer: Shuang Liang 7492259
Description: This is the controlling for switching between game levels
*/
#include "GameController.h"

GameLevel* GameController::currentLevel;
bool GameController::Loading;






/*
Fucntion: Init
Parameter: None
Return: None
Description: Initianting game levels
*/
void GameController::Init()
{
	Loading = true;
	currentLevel = 0;
}




/*
Fucntion: LoadInitialLevel
Parameter: GameLevel* lev (current level)
Return: None
Description: load current game level
*/
void GameController::LoadInitialLevel(GameLevel* lev)
{
	Loading = true; //This can help us avoid loading activity while rendering
	currentLevel = lev;
	currentLevel->Load();
	Loading = false;
}



/*
Fucntion: Graphics
Parameter: GameLevel* lev  (current level)
Return: None
Description: Constructor
*/
void GameController::SwitchLevel(GameLevel* lev)
{
	Loading = true;
	currentLevel->Unload();
	lev->Load();
	delete currentLevel;
	currentLevel = lev;
	Loading = false;
}




/*
Fucntion: Graphics
Parameter: None
Return: None
Description: Constructor
*/
void GameController::Render()
{
	if (Loading) return;//nice! Do not update or render if the scene is loading.
	currentLevel->Render();
}




/*
Fucntion: Graphics
Parameter: None
Return: None
Description: Constructor
*/
void GameController::Update()
{
	if (Loading) return; //nice! Do not update or render if the scene is loading.
	currentLevel->Update();
}


/*
Fucntion: Graphics
Parameter: None
Return: None
Description: Constructor
*/
 void GameController::Control(int x, int y)
{
	if (Loading) return;
	currentLevel->Control(x, y);

}

 void GameController::SetTrap()
 {
	 if (Loading) return;

	 currentLevel->SetTrap();
	 
 }






