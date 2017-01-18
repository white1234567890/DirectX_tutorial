//Author You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/11
//Last update 2017/1/11

#define WIN32_LEAN_AND_MEAN
#define SHIFTED 0x8000

//To detect memory leak
//Å´Å´Å´Å´Å´Å´Å´Å´Å´Å´
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//Å™Å™Å™Å™Å™Å™Å™Å™Å™Å™

#include "Common.h"
#include "Graphics.h"

//Prototype function
int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int);
bool CreateMainWindow(HWND &, HINSTANCE , int);
LRESULT WINAPI WinProc(HWND , UINT , WPARAM , LPARAM);
bool AnotherInstance();

//Groval variable
HINSTANCE hinst;
HDC hdc;							//Handle to device context
TCHAR ch = ' ';					//Input charactor
RECT rect;						//Struct Rectangle
PAINTSTRUCT ps;				//Is used in WM_PAINT
bool vkKeys[255];			//Key state
Graphics *graphics;		//Graphics pointer

//Start windows apprication
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	//If build by debug mode, monitor memory leak
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if(AnotherInstance()) PostQuitMessage(0);

	MSG msg;
	HWND hwnd = NULL;

	//Create window
	if(!CreateMainWindow(hwnd, hInstance , nCmdShow)) return 1;

	try
	{
	//Create Graphics object
	graphics = new Graphics;
	//Initialize Graphics, and throw GameError
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);


	//Main loop
	int done = 0;

	//Initialize Direct3D

	while(!done)
	{
		//Check PeelMessage , WindowMessage
		if(PeekMessage(&msg , NULL , 0 , 0 , PM_REMOVE))
		{
			//Detect quit message
			if(msg.message == WM_QUIT) done = 1;

			//Decode message and translate WinProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//If not window message, flip display
			graphics->showBackbuffer();
		}
	}

	//Release memory before finishong loop
	SAFE_DELETE(graphics);

	return msg.wParam;
	}

	catch(const GameError &err)
	{
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}

	catch(...)
	{
		MessageBox(NULL, "Unknown error occurred in game", "Error", MB_OK);
	}

	//Release memory before force shut down
	SAFE_DELETE(graphics);

	return 0;
}

//Window event callback function
LRESULT WINAPI WinProc(HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam)
{
	int nVirtkey; //Virtual key code

	switch(msg)
	{
	case WM_DESTROY:
		//Send message to quit this program
		
		PostQuitMessage(0);
		return 0;

		//If input charactor from keyboard 
	case WM_CHAR:
		switch (wParam)//charactor is wParam
		{
		//case 0x08:	//BackSpace
		//case 0x09:	//Tab
		//case 0x0A:	//Linefeed
		//case 0x0D:	//CaridgeRetrun
		//case 0x1B:	//Escape

		//	//Beep, dont draw
		//	MessageBeep((UINT) -1);
		//	return 0;

		//	//Expressable Charctor
		//default:
		//	ch = (TCHAR)wParam; //Get charactor

		case ESC_KEY: //Pressed Esc key
			//Message to Windows exit this program
			PostQuitMessage(0);
			return 0;

			//Force WM_PAINT
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}

		//When necessary to redraw window
		//Get handle to device context
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//Get window size
		GetClientRect(hWnd , &rect);

		//Draw charactor in center of window
		TextOut(hdc, rect.right/2, rect.bottom/2, &ch, 1);
		EndPaint(hWnd, &ps);

		return 0;

		//If press key
	case WM_KEYDOWN:
		vkKeys[wParam] = true;
		switch(wParam)
		{
			//Shift key
		case VK_SHIFT:
			//Get left Shift key state
			nVirtkey = GetKeyState(VK_LSHIFT);
			//If left shift key is pressed
			if(nVirtkey & SHIFTED) vkKeys[VK_LSHIFT] = true;

			//Get right Shift key state
			nVirtkey = GetKeyState(VK_RSHIFT);
			//If right shift key is pressed
			if(nVirtkey & SHIFTED) vkKeys[VK_RSHIFT] = true;

			break;

			//Ctrl key
		case VK_CONTROL:
			//Get left Ctrl key state
			nVirtkey = GetKeyState(VK_LCONTROL);
			//If left Ctrl key is pressed
			if(nVirtkey & SHIFTED) vkKeys[VK_LCONTROL] = true;

			//Get right Ctrl key state
			nVirtkey = GetKeyState(VK_RCONTROL);
			//If right Ctrl key is pressed
			if(nVirtkey & SHIFTED) vkKeys[VK_RCONTROL] = true;

			break;
		}

		//Force WM_PAINT
		InvalidateRect(hWnd, NULL, TRUE);

		return 0;
		break;

		//If release key
	case WM_KEYUP:
		vkKeys[wParam] = false;
		switch (wParam)
		{
			//Shift key
		case VK_SHIFT:
			//Get left Shift key state
			nVirtkey = GetKeyState(VK_LSHIFT);
			//If left shift key is released
			if((nVirtkey & SHIFTED) == 0) vkKeys[VK_LSHIFT] = false;

			//Get right Shift key state
			nVirtkey = GetKeyState(VK_RSHIFT);
			//If right shift key is released
			if((nVirtkey & SHIFTED) == 0) vkKeys[VK_RSHIFT] = false;

			break;

			//Ctrl key
		case VK_CONTROL:
			//Get left Ctrl key state
			nVirtkey = GetKeyState(VK_LCONTROL);
			//If left Ctrl key is released
			if((nVirtkey & SHIFTED) == 0) vkKeys[VK_LCONTROL] = false;

			//Get right Ctrl key state
			nVirtkey = GetKeyState(VK_RCONTROL);
			//If right Ctrl key is released
			if((nVirtkey & SHIFTED) == 0) vkKeys[VK_RCONTROL] = false;

			break;
		}

		//Force WM_PAINT
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

//If error return : false
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance , int nCmdShow)
{
	WNDCLASSEX wcx;

	//Set WNDCLASSEX to use main window
	wcx .cbSize = sizeof(wcx);													//Struct size
	wcx.style = CS_HREDRAW | CS_VREDRAW;									//Redraw when window size was changed
	wcx.lpfnWndProc = WinProc;													//Pointer to window procedure
	wcx.cbClsExtra = 0;																//Extension class memory is none
	wcx.cbWndExtra = 0;																//Extension window memory is none
	wcx.hInstance = hInstance;													//Handle to instance
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL , IDC_ARROW);							//Default cursol
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//Brack back screen
	wcx.lpszMenuName = NULL;														//Menu resource name
	wcx.lpszClassName = CLASS_NAME;											//Window class name
	wcx.hIconSm = NULL;																//Small icon

	//Register window class
	//If Error RegisterClassEx return 0
	if(RegisterClassEx(&wcx) == 0) return false;

	//Set window or fullscreen
	DWORD style;
	if(FULLSCREEN)
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else
	{
		style = WS_OVERLAPPEDWINDOW;
	}

	//Create window
	hwnd = CreateWindow(
		CLASS_NAME,				//Window class name
		APP_TITLE,					//Title ber
		WS_OVERLAPPEDWINDOW,	//Window style
		CW_USEDEFAULT,			//Default of window horizontal location
		CW_USEDEFAULT,			//Default of window vertical locaiton
		GAME_WIDTH,				//Window horizontal size
		GAME_HEIGHT,			//Window vertical size
		(HWND)NULL,				//Nothing parent window
		(HMENU)NULL,				//Nothing menu
		hInstance,					//Handle to apprication instance
		(LPVOID)NULL);			//Nothing window parameter

	//If error to create window
	if(!hwnd) return false;

	//Process that window
	if (!FULLSCREEN)
	{
		//Adjust window size that crient area siza is GAME_WIDTH * GAME_HEIGHT
		RECT clientRect;

		//Get client size
		GetClientRect(hwnd, &clientRect);

		//Redraw window
		MoveWindow(hwnd,
			0,													//Left
			0,													//Top
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),		//Right
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom),	//Bottom
			TRUE);
	}

	//Draw window
	ShowWindow(hwnd , nCmdShow);

	//Send message WM_PAINT to window procedure
	UpdateWindow(hwnd);

	return true;
}

//This apprecation is running already
//If Discover another instance : return true
//else : return false
bool AnotherInstance()
{
	HANDLE ourMutex;

	//Try to create mutex in unique string
	ourMutex = CreateMutex(NULL, true,
		"Use_a_different_string_here_for_each_program_48161-XYZZY");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//Release mutex
		ReleaseMutex(ourMutex);
		//Discover another instance
		return true;
	}

	//This is only instance
	return false;
}