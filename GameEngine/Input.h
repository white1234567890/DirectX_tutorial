//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/1/19
//LastUpdate 2017/1/19

#pragma once
#include <Windows.h>
#include <XInput.h>
#include "GameError.h"


#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)

namespace inputNS
{
	const int KEYS_ARRAY_LEN = 255;
}

const int MAX_CONTROLLERS = 4;

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

	void checkController();
};

