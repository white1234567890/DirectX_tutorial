//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/1/19
//LastUpdate 2017/1/19

#pragma once
#include <Windows.h>
#include <XInput.h>

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
};

class Input
{
private:  
	bool keysDown[inputNS::KEYS_ARRAY_LEN];			//If specified keys are pressed, this variable is true
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];		//When specified keys are pressing, this variable is true
	std::string textIn;								//Text inputed by user
	char charIn;									//Last inputed charactor
	bool newLine;									//When start new line, this variable is true
	int mouseX,mouseY;								//Mouse location in window
	int mouseRawX,mouseRawY;						//High-definition mouse location in window
	RAWINPUTDEVICE Rid[1];							//For High definition mouse
	bool mouseCaptured;								//If mouse was captured, this variable is true
	bool mouseLButton;								//If mouse left button is pressed, this variable is true
	bool mouseMButton;								//If mouse middle button is pressed, this variable is true
	bool mouseRButton;								//If mouse right button is pressed, this variable is true
	bool mouseX1Button;								//If mouse X1 button is pressed, this variable is true
	bool mouseX2Button;								//If mouse X2 button is pressed, this vaiable is true
	ControllerState controllers[MAX_CONTROLLERS];	//Contrpller state	

public:
	Input(void);
	~Input(void);


};

