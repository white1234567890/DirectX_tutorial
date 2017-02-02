//You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/19
//LastUpdate 2017/2/2

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>

//My class
#include "Graphics.h"
#include "Input.h"
#include "constants.h"
#include "GameError.h"

//Game class contains variables and functions commonly used in game
//Game class is base class to inherite in main class used in game
//Game class contains Input class object and Graphics class object
//So, This class contains functions drawing graphics and getting input from keyboard
class Game
{
protected:
	//Common game property
	Graphics *graphics;			//Graphics pointer
	Input *input;						//Input pointer
	HWND hwnd;							//Window handle
	HRESULT hr;						//Default return value
	LARGE_INTEGER timeStart;	//Performance counter initial value
	LARGE_INTEGER timeEnd;		//Performance counter last value
	LARGE_INTEGER timerFreq;	//Performance counter frequency
	float frameTime;				//Last frame time
	float fps;							//Frame rate (frame per a second)
	DWORD sleepTime;				//Sleep time between frame and frame
	bool paused;						//If game is paused, true
	bool initialized;				//If game is initialized, true

public:
	//Constructor
	//Constructor create Input class object to access keyboard as soon as
	//Then, call input->initialize and finish input preparation to enable accessing to mouse
	Game(void);

	//Destructor
	~Game(void);

	//Member function
	//««««««««««

	//Windows message handler
	//This function make accessing to function in Input class object easy
	//Keyboard and mouse are processed through Windows event
	//When Windows detect keyboard event or mouse event, this function is called to process message
	//Then, messageHandler function call proper function in Input class object, and process input data
	//wParam : store key data
	//lParam : store mouse data
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//Initialize game
	//Before run : hwnd is handle to window
	//
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

	//If lost graphics device, restore it
	//Checking lost graphics device and processing it execute in this function
	virtual void handleLostGraphicsDevice();

	//Return pointer to Graphics
	Graphics* getGraphics(){return graphics;}

	//Return pointer to Input
	Input* getInput(){return input;}

	//Exit game
	void exitGame(){PostMessage(hwnd, WM_DESTROY, 0, 0);}

	//Pure virtual function
	//Pure virtual function works like a place holder
	//These functions need to be overridden in class to be inherited in
	//Game class does not know this game's detail, these function is pure virtual function
	//««««««««««

	//Update game item
	//This function need to override
	virtual void update() = 0;

	//Run AI
	//This function need to override
	virtual void ai() = 0;

	//Check collisions
	//This function need to override
	virtual void collisions() = 0;

	//Render graphics
	//Call graphics->spriteBegin();
	//Render sprite
	//Call graphics->spriteEnd();
	//Render others
	//This function need to override
	virtual void render() = 0;

	//ªªªªªªªªªª

	//ªªªªªªªªªª
};