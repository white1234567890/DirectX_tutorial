//You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/19
//LastUpdate 2017/1/19

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>

//My class
#include "Graphics.h"
#include "Input.h"
#include "constants.h"
#include "GameError.h"

class Game
{
protected:
	//Common game property
	Graphics *graphics;			//Graphics pointer
	Input *input;				//Input pointer
	HWND hwnd;					//Window handle
	HRESULT hr;					//Default return value
	LARGE_INTEGER timeStart;	//Performance counter initial value
	LARGE_INTEGER timeEnd;		//Performance counter last value
	LARGE_INTEGER timerFreq;	//Performance counter frequency
	float frameTime;			//Last frame time
	float fps;					//Frame rate (frame per a second)
	DWORD sleepTime;			//Sleep time between frame and frame
	bool paused;				//If game is paused, true
	bool initialized;			//If game is initialized, true

public:
	//Constructor
	Game(void);

	//Destructor
	~Game(void);

	//Member function
	//««««««««««

	//Windows message handler
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//Initialize game
	//Before run : hwnd is handle to window
	virtual void initialize(HWND hwnd);

	//run is recurring in WinMain message loop
	virtual void run(HWND);

	//When graphic device vanished, call this function
	//Release all video memories to be available reset graphic device
	virtual void releaseAll();

	//Recreate all surface, and reset all entity
	virtual void resetAll();

	//Release all memories
	virtual void deleteAll();

	//Render game item
	virtual void renderGame();

	//If lost graphics device, research it;
	virtual void handleLostGraphicsDevice();

	//Return pointer to Graphics
	Graphics* getGraphics(){return graphics;}

	//Return pointer to Input
	Input* getInput(){return input;}

	//Exit game
	void exitGame(){PostMessage(hwnd, WM_DESTROY, 0, 0);}

	//Pure virtual function
	//These functions need to rewrite in class inherited this class
	//««««««««««

	//Update game item
	virtual void update() = 0;

	//Run AI
	virtual void ai() = 0;

	//Check collisions
	virtual void collisions() = 0;

	//Render graphics
	//Call graphics->spriteBegin();
	//Render sprite
	//Call graphics->spriteEnd();
	//Render others
	virtual void render() = 0;

	//ªªªªªªªªªª

	//ªªªªªªªªªª
};