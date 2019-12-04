/*
File: main.cpp
Project: SET Hunter
Programmer: Shuang Liang  7492259
Description: This is the main game loop
*/
#include <Windows.h>
#include "Graphics.h"
#include "Level1.h"
#include "GameController.h"
#include "soundclass.h"

Graphics* graphics;


#define START_BUTTON 201
#define END_BUTTON 404




//The window handlers for the two different windows
HWND menuHandler;
HWND gameHandler;





/*
function: StartProc
Parameter:HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
Return: LRESULT CALLBACK
description: This is the call back function for creating the starting page
*/
LRESULT CALLBACK StartProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{

	switch (uMsg)
	{
	case WM_CREATE:
	{
	 
		HINSTANCE hIns = ((LPCREATESTRUCT)lParam)->hInstance;
		CreateWindowEx(0, "static", "Welcome to SET Hunter:  Agent Shuang", WS_CHILD | WS_VISIBLE| SS_CENTER, 100, 100, 400, 300, hwnd, NULL, hIns, NULL);
	
		CreateWindowEx(0, "button", "Start Game", WS_CHILD | WS_VISIBLE, 350, 300, 300, 80, hwnd, (HMENU)START_BUTTON, hIns, 0);
		CreateWindowEx(0, "button", "End Game", WS_CHILD | WS_VISIBLE, 350, 400, 300, 80, hwnd, (HMENU)END_BUTTON, hIns, 0);

		return 0;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == START_BUTTON)
		{
			//MessageBox(menuWindowHandle, "button  has  been clicked", "title", MB_ICONINFORMATION);
			ShowWindow(menuHandler, SW_HIDE);
			ShowWindow(gameHandler, SW_SHOWDEFAULT);

		}
		else if (LOWORD(wParam) == END_BUTTON)
		{
			DestroyWindow(menuHandler);
		}
		break;
	}
	/*case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(menuHandler, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));
		EndPaint(menuHandler, &ps);
		return 0;
	}*/
	case WM_CLOSE:
	{
		if (MessageBox(menuHandler, "Do you want to exit?", "EXIT", MB_OKCANCEL) == IDOK)
			DestroyWindow(menuHandler);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}


	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}








/*
function: GameProc
Parameter:  HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam 
Return:   RESULT CALLBACK
description: This is the call back function for creating the game window
*/
LRESULT CALLBACK GameProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	if (uMsg == WM_DESTROY) { PostQuitMessage(0); return 0; }
	
//BIG CHANGE! We are not Drawing/rendering here! 
// We've moved this down to the message handling loop below

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



int InitStartWindow(HINSTANCE hInstance)
{
	//initilaing the startpage window
	WNDCLASSEX StartPage;
	ZeroMemory(&StartPage, sizeof(WNDCLASSEX));
	StartPage.cbSize = sizeof(WNDCLASSEX);
	StartPage.hbrBackground = (HBRUSH)COLOR_WINDOW;
	StartPage.hInstance = hInstance;
	StartPage.lpfnWndProc = StartProc;
	StartPage.lpszClassName = "StartPage";
	StartPage.style = CS_HREDRAW | CS_VREDRAW; //Alert - This is useful here... what does it do?

	RegisterClassEx(&StartPage);


	RECT startRect = { 0, 0, 10 * GRID_L, 10 * GRID_H };//Do these numbers look significant to you? What are they?
	AdjustWindowRectEx(&startRect, WS_OVERLAPPED, false, WS_EX_OVERLAPPEDWINDOW);

	//Below is another important process to understand... what are we doing?
	//Why is this connected to rect we just defined above?
	menuHandler = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "StartPage", "SET Hunter", WS_OVERLAPPEDWINDOW, 100, 100,
		startRect.right - startRect.left, startRect.bottom - startRect.top, NULL, NULL, hInstance, 0);
	if (!menuHandler) return -1;
}

int InitGameWindow(HINSTANCE hInstance)
{
	//initializing the game windows
	WNDCLASSEX windowclass;
	ZeroMemory(&windowclass, sizeof(WNDCLASSEX));
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = GameProc;
	windowclass.lpszClassName = "MainWindow";
	windowclass.style = CS_HREDRAW | CS_VREDRAW; //Alert - This is useful here... what does it do?

	RegisterClassEx(&windowclass);


	RECT rect = { 0, 0, 10 * GRID_L, 10 * GRID_H };//Do these numbers look significant to you? What are they?
	AdjustWindowRectEx(&rect, WS_OVERLAPPED, false, WS_EX_OVERLAPPEDWINDOW);

	//Below is another important process to understand... what are we doing?
	//Why is this connected to rect we just defined above?
	gameHandler = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MainWindow", "SET Hunter", WS_OVERLAPPEDWINDOW, 100, 100,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);


	//initialize sound with the game window handler


	Level1::Sound->Initialize(gameHandler);



	if (!gameHandler) return -1;

}


///Here is the main
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPWSTR cmd,
	int nCmdShow
)
{

	InitStartWindow(hInstance);


	InitGameWindow(hInstance);

	Level1::Sound->PlayWaveFile(true, SoundClass::SoundColletion::Background);
	
	//PlaySound("background.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);


	graphics = new Graphics();
	if (!graphics->Init(gameHandler))
	{
		delete graphics;
		return -1;
	}


	GameLevel::Init(graphics);


	ShowWindow(menuHandler, SW_SHOWDEFAULT);
	ShowWindow(gameHandler, SW_HIDE);

	//ShowWindow(windowhandle, nCmdShow);
	GameController::LoadInitialLevel(new Level1());

#pragma region GameLoop
	//Below, we have essentially an infinite loop that will keep the window running and will dispatch/show messages
	//As many people will tell you, most Windows you see are just infinite loops waiting for some kind of work-flow or 
	//system-based interuption.

	//Note - Our message handling has changed from the first demo code.
	//Here, we use a PeekMessage to avoid locking the graphics/windoProc
	//when a message has to be dispatched.
	MSG message;
	message.message = WM_NULL; //Do not have this set to WM_QUIT, which has a specific context
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			//This allows us to send a message to the WindowProc IF there is one
			DispatchMessage(&message);
		else
		{
			switch (message.message)
			{

			case WM_KEYDOWN:
					switch (message.wParam)
					{
					case VK_UP:

						GameController::Control(0, -Car::VehicleMovementRate);
						break;
					case VK_DOWN:
						GameController::Control(0, Car::VehicleMovementRate);

						break;
					case VK_RIGHT:
						GameController::Control(Car::VehicleMovementRate, 0);

						break;
					case VK_LEFT:
						GameController::Control(-Car::VehicleMovementRate, 0);

						break;

					case VK_SPACE:
						GameController::SetTrap();

					}

			default:

				GameController::Update();
				//Render Routine... This is very modular. GameController now handles the rendering
				graphics->BeginDraw();
				GameController::Render();
				graphics->EndDraw();





				break;
			}
		     	

		}


		if (Level1::endOfGame)
		{
			Level1::endOfGame = false;

			//back to menu show the final score

			/*ShowWindow(menuHandler, SW_SHOWDEFAULT);
			ShowWindow(gameHandler, SW_HIDE);*/

			
		
		}

	}

#pragma endregion
	delete graphics;
	return 0;
}








