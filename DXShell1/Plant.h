#pragma once
#include <string>
#include "Graphics.h"
#include "GameController.h"
#include <string>


/*
Class: PLant
Des: this is the collection of all randomly generated plants
*/
class Plant
{

private:
	string Name;
	SpriteSheet* PlantSheet;


public:
	static float topSpeed() { return 24; }
	static float defaultSpeed() { return 8; }
	static int treeAddY;
	static float acceleration;

	Plant(string plantName, Graphics* gfx)
	{
		this->Name = plantName;
		
		if (plantName == "tree1") this->PlantSheet = new SpriteSheet(L"tree1.bmp", gfx);
		if(plantName == "tree2") this->PlantSheet = new SpriteSheet(L"tree2.bmp", gfx);
		if(plantName == "shrub") this->PlantSheet = new SpriteSheet(L"shrub1.bmp", gfx);

		
	}

	string GetName() { return this->Name; }

	SpriteSheet* GetPlantSheet() { return this->PlantSheet; }

};


