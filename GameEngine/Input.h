//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/1/19
//LastUpdate 2017/1/19

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <windowsx.h>
#include <XInput.h>
#include "constants.h"
#include "GameError.h"

//for high-definition mouse

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#endif // HID_USAGE_PAGE_GENERIC
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif // HID_USAGE_GENERIC_MOUSE

namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256;

	//Set All keysDown array false
	const UCHAR KEYS_DOWN = 1;

	//Set all keysPressed array false
	const UCHAR KEYS_PRESSED = 2;

	//Set mouse X and Y 0
	const UCHAR MOUSE = 4;

	//Clear input text string
	const UCHAR TEXT_IN = 8;

	//Clear all input buffer
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

const int MAX_CONTROLLERS = 1;

typedef struct ControllerState
{
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	float vibrationTimeLeft;	//Miri second
	float vibrationTimeRight;	//Miri second
	bool connected;
}ControllerState;

class Input
{
private:  
	bool keysDown[inputNS::KEYS_ARRAY_LEN];				//If specified keys are pressed, this variable is true
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];			//When specified keys are pressing, this variable is true
	std::string textIn;												//Text inputed by user
	char charIn;														//Last inputed charactor
	bool newLine;														//When start new line, this variable is true
	int mouseX,mouseY;												//Mouse location in window
	int mouseRawX,mouseRawY;									//High-definition mouse location in window
	RAWINPUTDEVICE Rid[1];										//For High definition mouse
	bool mouseCaptured;											//If mouse was captured, this variable is true
	bool mouseLButton;												//If mouse left button is pressed, this variable is true
	bool mouseMButton;												//If mouse middle button is pressed, this variable is true
	bool mouseRButton;												//If mouse right button is pressed, this variable is true
	bool mouseX1Button;											//If mouse X1 button is pressed, this variable is true
	bool mouseX2Button;											//If mouse X2 button is pressed, this vaiable is true
	ControllerState controllers[MAX_CONTROLLERS];	//Contrpller state	

public:
	Input(void);
	~Input(void);

	//Set array of RAWINPUTDEVICE Rid
	//Resolution of standard conputer mouse is 400 dpi(dot per inch), but high-definition mouse is more than 800 dpi
	//High-definition mouse can not get from default WM_MOUSEMOVE message
	//If necessary capture mouse, clear array stored controller state
	//If mouse is captured, mouse input send to capture window
	//This is necessary game that player move is controled by mouse
	//Standard mouse bihavior do not need mouse capture
	void initialize(HWND hwnd, bool capture);

	void keyDown(WPARAM wParam);
	void keyUp(WPARAM wParam);
	void keyIn(WPARAM wParam);
	bool isKeyDown(UCHAR vkey) const;
	bool wasKeyPressed(UCHAR vkey) const;
	bool anyKeyPressed(UCHAR vkey) const;
	
	//Return input text as string
	std::string getTextIn(){return textIn;}

	//Return input last charactor
	char getCharIn(){return charIn;}

	//Clear specified key pressed state
	void clearKeyPress(UCHAR vkey);

	//Clear text buffer
	void clearTextIn(){textIn.clear();}
	
	//Clear specified input buffer
	//Param what : call from input_NS, and conbine any item to use OR operator(|) 
	//Example : input->clear(inputNS::KEYS_DOWN | input_NS::KEYS_PRESSED)
	void clear(UCHAR what);

	//Clear data keys, mouse, and input text state
	void clearAll(){clear(inputNS::KEYS_MOUSE_TEXT);}

	void checkControllers();
};

