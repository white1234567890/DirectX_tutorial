//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/1/19
//LastUpdate 2017/2/3

#include "Game.h"

Game::Game(void)
{
	//Scures memory and initialize Input
	input = new Input();

	//Any other initialization will call input->initialize() after

	paused = false;
	graphics = NULL;
	initialized = false;
}


Game::~Game(void)
{
	//Release all memories
	deleteAll();

	ShowCursor(true);
}

//////////////////////////////////////////////////////////////////////////////
//Windows message handler
//////////////////////////////////////////////////////////////////////////////
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(initialized)	//If did not initialize, do not process message
	{
		switch (msg)
		{
		case WM_DESTROY:
			//Send message to quit this program to Windows
			PostQuitMessage(0);
			return 0;

			//Key is pressed
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			input->keyDown(wParam);
			return 0;

			//Key is released
		case WM_KEYUP:
		case WM_SYSKEYUP:
			input->keyUp(wParam);
			return 0;

			//Input charactor
		case WM_CHAR:
			input->keyIn(wParam);
			return 0;

			//Mouse was moved
		case WM_MOUSEMOVE:
			input->mouseIn(lParam);
			return 0;

			//Input raw data from mouse
		case WM_INPUT:
			input->mouseRawIn(lParam);
			return 0;

			//Mouse left button is pressed
		case WM_LBUTTONDOWN:
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;

			//Mouse left button is released
		case WM_LBUTTONUP:
			input->setMouseLButton(false);
			input->mouseIn(lParam);
			return 0;

			//Mouse middle button is pressed
		case WM_MBUTTONDOWN:
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;

			//Mouse middle button is released
		case WM_MBUTTONUP:
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;

			//Mouse right button is pressed
		case WM_RBUTTONDOWN:
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;

			//Mouse right button is released
		case WM_RBUTTONUP:
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;

			//Mouse X button is pressed
		case WM_XBUTTONDOWN:
			input->setMouseXButton(true);
			input->mouseIn(lParam);
			return 0;

			//Mouse X button is released
		case WM_XBUTTONUP:
			input->setMouseXButton(false);
			input->mouseIn(lParam);
			return 0;

			//Check controller
		case WM_DEVICECHANGE:
			input->checkControllers();
			return 0;
		}
	}

	//Entrust processing Windows
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////////
//Initialize game
//When error, throw GameError
//////////////////////////////////////////////////////////////////////////////
void Game::initialize(HWND hw)
{
	hwnd = hw; //Save window handle

	//Initialize graphics
	graphics = new Graphics();

	//Throw GameError
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	//Initialize input, do not capture mouse
	//Throw GameError
	input->initialize(hwnd, false);

	//Try to prepare using high resolution timer
	if(QueryPerformanceFrequency(&timerFreq) == false)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
	}

	//Get Start time;
	QueryPerformanceCounter(&timeStart);

	initialized = true;
}

void Game::handleLostGraphicsDevice()
{
	//Test device was lost, and execute apropriate processing
	hr = graphics->getDeviceState();

	//If graphics device is disabled
	if(FAILED(hr))
	{
		//Device was lost, and it can not reset
		if(hr == D3DERR_DEVICELOST)
		{
			//Surrender CPU time (100 miri second)
			Sleep(100);
			return;
		}
		//Device was lost, but it can reset
		else if(hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			//Try to reset graphics device
			hr = graphics->reset();
			//If reset failed
			if(FAILED(hr)) return;
			resetAll();
		}
		//Other devices error
		else return;	
	}
}

//Render game items
void Game::renderGame()
{
	//Start rendering
	if(SUCCEEDED(graphics->beginScene()))
	{
		//render() is pure virtual function that need to override in inherited class
		//Call render() in inherited class 
		render();

		//Finish rendering
		graphics->endScene();
	}

	handleLostGraphicsDevice();
	//Draw back buffer on window
	graphics->showBackbuffer();
}
