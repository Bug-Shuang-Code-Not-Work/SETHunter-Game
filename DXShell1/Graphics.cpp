/*
Project: SET Hunter
File: Graphics.cpp
Programmer: Shuang Liang 7492259
Description: This is the implemntation for drawing grass and road 
*/

#include "Graphics.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#pragma warning (disable:4996)
/***********************************************************************************
The intent of the Graphics class is to handle our DirectX calls, and to be largely responsible 
for managing the rendertarget.
******************************************************************************************/

//Constructor for Graphics class

/*
Fucntion: Graphics
Parameter: None 
Return: None
Description: Constructor
*/


float Graphics::bottomX = 0.3 * XGridNum* GRID_L;
float Graphics::bottomY = YGridNum * GRID_H;
float Graphics::topPointX = 0.3 * XGridNum* GRID_L;
int Graphics::currentDirection = Straight;
float Graphics::roadMovement = 1.0f;
float Graphics::roadAddY = 0;


Graphics::Graphics()
{
	factory = NULL;
	rendertarget = NULL;
	brush = NULL;


}

//Destructor for Graphics class
//Note that all COM objects we instantiate should be 'released' here 
//Look for comments on COM usage in the corresponding header file.


/*
Fucntion: ~Graphics
Parameter: None
Return: None 
Description: Desctructor
*/
Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (rendertarget) rendertarget->Release();
	if (brush) brush->Release();

}






/*
Fucntion: Init
Parameter:  HWND windowHandle
Return: true if succeed, otherwise false
Description: This method initiate graphics elemtnents like windows size brush and rend targets
*/
bool Graphics::Init(HWND windowHandle)
{
	// init factory
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;

	//create rectangle
	RECT rect;
	GetClientRect(windowHandle, &rect); //set the rect's right and bottom properties = the client window's size

	//init render target
	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&rendertarget);
	if (res != S_OK) return false;
	
	//init brush
	res = rendertarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
	if (res != S_OK) return false;


	//init write factory interface
	res = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory_)
	);
	if (res != S_OK) return false;

	//init text format interface
	res = pDWriteFactory_->CreateTextFormat(
		L"Gabriola",                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		40.0f,
		L"en-us",
		&pTextFormat_
	);
	if (res != S_OK) return false;



	//center the text 
	res = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	if (res != S_OK) return false;

	res = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	if (res != S_OK) return false;


	return true;
}




/*
Fucntion: ClearScreen
Parameter:float r, float g, float b color of the background
Return: None
Description:  Creates the background
*/
void Graphics::ClearScreen(float r, float g, float b) 
{
	rendertarget->Clear(D2D1::ColorF(r, g, b));
}





/*
Fucntion: DrawRoad
Parameter:float addY
Return: None
Description:  Draw the curvy road
*/
void Graphics::DrawRoad(float addY)
{

	HRESULT hr = factory->CreatePathGeometry(&pathGeometry);
	
	//first point
	float firstPointX = bottomX  + roadMovement;
	float firstPointY = 3 * GRID_H;

	//second point
	float secondPointX = topPointX;
	float secondPointY = 7 * GRID_H;


	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* pSink = NULL;

		hr = pathGeometry->Open(&pSink);
		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

			//Bottom Point
			pSink->BeginFigure(
				D2D1::Point2F(bottomX + roadMovement, bottomY + addY),
				D2D1_FIGURE_BEGIN_FILLED
			);

			// Middle two points in the screen and top points for the left edge line
			pSink->AddBezier(
				D2D1::BezierSegment(
					D2D1::Point2F(secondPointX, secondPointY),
					D2D1::Point2F(firstPointX, firstPointY),
				
					D2D1::Point2F(topPointX, 0)
				));

			//the top connecting line
			pSink->AddLine(D2D1::Point2F((topPointX + roadWidth()), 0));
			

			// the right edge line
			pSink->AddBezier(
				D2D1::BezierSegment(
					D2D1::Point2F(firstPointX + roadWidth(), firstPointY),
					D2D1::Point2F(secondPointX + roadWidth(), secondPointY),
								
					D2D1::Point2F(bottomX + roadWidth() + roadMovement, bottomY + addY)
				)
			);

			pSink->EndFigure(D2D1_FIGURE_END_OPEN);

			hr = pSink->Close();
			pSink->Release();
		}
	}


	brush->SetColor(D2D1::ColorF(0, 0, 0, 1));
	
	rendertarget->FillGeometry(pathGeometry, brush, 0);
	rendertarget->DrawGeometry(pathGeometry, brush, 1.0);


}


/*
Fucntion: UpdateBezierPoints
Parameter:  int currentDirection
Return: None 
Description: this method update the control points for beizeir curve
*/
void Graphics::UpdateBezierPoints(int currentDirection, int treeAddy)
{


	if (treeAddy == 0)
	{
		return;
	}


	if (currentDirection == Straight)
	{
		if (topPointX > (bottomX + roadMovement))
		{
			roadMovement = roadMovement + 1.0f;
		}

			

		else
		{

			if (topPointX == (bottomX + roadMovement))
			{
				roadAddY = 0;
			}

	
			roadMovement = roadMovement - 1.0f;
		}


	}

	else if (currentDirection == Left)
	{
		if (topPointX > LeftEdge() && (bottomX + roadMovement) > LeftEdge())
		{
			topPointX -= 2.0f;

		}
		else
		{
			if (bottomX + roadMovement > topPointX)
			{
				roadMovement -= 1.0f;
			}
		}

	}
	else if (currentDirection == Right)
	{
		if (topPointX < RightEdge() && (bottomX + roadMovement) < RightEdge())
		{
			topPointX += 2.0f;
		}
		else
		{
			if (bottomX + roadMovement < topPointX)
			{
				roadMovement += 1.0f;
			}
		}
	}


}




/*
Fucntion: CrateNewDirection
Parameter: None
Return: int new direction
Description: It creates a new direction based on random chance
*/
int Graphics::CrateNewDirection()
{
	time_t seed;
	srand((unsigned)time(&seed));


	int randNum;



	randNum = rand() % 3 - 1;


	if (randNum == Left && topPointX == LeftEdge())
	{
		return Straight;
	}

	else if (randNum == Right && topPointX == RightEdge())
	{
		return Straight;
	}


	return randNum;

}






bool Graphics::WriteText(unsigned long Scores, int lifeRemained)
{
	//create a new brush
	ID2D1SolidColorBrush* textBrush;

	HRESULT res = rendertarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::GhostWhite),
		&textBrush
	);
	if (res != S_OK) return false;



	//convert unsigned long to string and append to make ouput string
	char scoreText[9] = "Scores: ";
	char scoreBuffer[sizeof(unsigned long) * 8 + 1];
	ultoa(Scores, scoreBuffer, 10);

	char* scoreShow = (char*)malloc(strlen(scoreText) + strlen(scoreBuffer) + 1);
	strcpy(scoreShow, scoreText);
	strcat(scoreShow, scoreBuffer);

	char middleSpace[9] = "        ";

	char lifeText[10] = "Life: ";
	char lifeBuffer[5];
	itoa(lifeRemained, lifeBuffer, 10);
	
	strcat(lifeText, lifeBuffer);

	
	char* completeShow = (char*)malloc(strlen(scoreShow) + strlen(lifeText) + 9 + 1);
	strcpy(completeShow, scoreShow);
	strcat(completeShow, middleSpace);
	strcat(completeShow, lifeText);


	//text variables
	size_t length = strlen(completeShow) + 1;
	const size_t newSize = 100;
	size_t convertedChars = 0;
	wchar_t wszText_[newSize];
	mbstowcs_s(&convertedChars, wszText_, length, completeShow, _TRUNCATE);
	wcscat_s(wszText_, L"");

	UINT32 cTextLength_;	

	//convert to special format
	//mbstowcs(wszText_, completeShow, strlen(completeShow)+1);


	//layout rectangle
	D2D1_RECT_F layoutRect = D2D1::Rect(
		0.2*GRID_L*XGridNum,
		0.05*GRID_H*YGridNum,
		0.8*GRID_L*XGridNum,
		0.1*GRID_H*YGridNum
	);


	//information to be printed on screen
	cTextLength_ = (UINT32)wcslen(wszText_);


	rendertarget->DrawText(
		wszText_,        // The string to render.
		cTextLength_,    // The string's length.
		pTextFormat_,    // The text format.
		layoutRect,       // The region of the window where the text will be rendered.
		textBrush     // The brush used to draw the text.
	);

	free(scoreShow);
	free(completeShow);


	return true;
}
