/*
Project: SET Hunter
File: Graphics.h
Programmer: Shuang Liang 7492259
Description: This is the class definition for Graphics 
*/


#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")

#include<Windows.h>
#include <d2d1_1.h>
#include <d2d1.h>
#include <d3d11_1.h>
#include <d2d1effects.h>
#include <d2d1effects_2.h>
#include <d2d1effecthelpers.h>
#include <vector>
#include <map>
#include <dwrite.h>
using namespace std;

// grid size
#define GRID_L 102.4f
#define GRID_H 76.8f

//how many grids
static const int XGridNum = 10;
static const int YGridNum = 10;


// the road direction
enum Direction { Left = -1, Straight, Right };



class Graphics
{
	//Below, these are all COM interfaces we're using to create D2D resources.
	//We release them as part of the ~Graphics deconstructor... or bad things can happen
	ID2D1Factory* factory; //The factory allows us to create many other types of D2D resources
	ID2D1HwndRenderTarget* rendertarget; //this is typically an area in our GPU memory.. like a back buffer 
	ID2D1SolidColorBrush* brush; //Note this COM interface! Remember to release it!
	

	//for drawing path
	ID2D1PathGeometry* pathGeometry;



	// for drawing text
	IDWriteFactory* pDWriteFactory_;
	IDWriteTextFormat* pTextFormat_;




public:





	//for drawing road
	static float LeftEdge() { return 200; }
	static float RightEdge() { return 500; }
	static float roadWidth() { return 4 * GRID_L; }


	static float bottomX; 
	static float bottomY;
	static float topPointX; 
	static int currentDirection; 
	static float roadMovement; 
	static float roadAddY; 



	Graphics();

	~Graphics();

	bool Init(HWND windowHandle);

	ID2D1RenderTarget* GetRenderTarget()
	{
		return rendertarget;
	}

	ID2D1DeviceContext* GetDeviceContext()
	{
		ID2D1DeviceContext *rtDC;
		rendertarget->QueryInterface(&rtDC);
		return rtDC;
	}

	void BeginDraw() { rendertarget->BeginDraw(); }
	void EndDraw() { rendertarget->EndDraw(); }

	void ClearScreen(float r, float g, float b);


	void Graphics::DrawRoad( float addY);


	void Graphics::UpdateBezierPoints(int currentDirection, int treeAddy);
	int Graphics::CrateNewDirection();


	bool WriteText(unsigned long Scores, int lifeRemained);



};
