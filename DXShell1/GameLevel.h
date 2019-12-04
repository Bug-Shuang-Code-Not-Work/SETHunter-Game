/*
Project: SET Hunter
File: GameLevel.h
Programmer: Shuang Liang 7492259
Description: This is class definition for virtual class game Level
*/

#pragma once

#include "Graphics.h"
#include "SpriteSheet.h"

class GameLevel
{
protected:
	static Graphics* gfx;

public:
	static void Init(Graphics* graphics)
	{
		gfx = graphics;
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Control(int x, int y) = 0;
	virtual void SetTrap() = 0;
};