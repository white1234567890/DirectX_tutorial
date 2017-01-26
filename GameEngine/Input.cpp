#include "Input.h"

//////////////////////////////////////////////////////////////////////////////
//Default constructor
//////////////////////////////////////////////////////////////////////////////
Input::Input(void)
{
	//Clear array that key is pressed state
	for(size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
	{
		keysDown[i] = false;
	}

	//Clear array that key is pressing state
	for(size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
	{
		keysPressed[i] = false;
	}

	newLine = true;	//Start new line
	textIn = "";		//Clear textIn
	charIn = 0;			//Clear charIn

	//Mouse data
	mouseX = 0;	//Window X axis
	mouseY = 0;	//Window Y axis
	mouseRawX = 0;	//High-definition X axis
	mouseRawY = 0;	//High-definition Y axis
	mouseLButton = false;	//If mouse left button is pressed, this variable is true
	mouseMButton = false;	//If mouse middle button is pressed, this variable is true
	mouseRButton = false;	//If mouse right button is pressed, this variable is true
	mouseX1Button = false;	//If mouse X1 button is pressed, this variable is true
	mouseX2Button = false;	//If mouse X2 button is pressed, this variabe is true

	for(int i = 0; i < MAX_CONTROLLERS; i++)
	{
		controllers[i].vibrationTimeLeft = 0;
		controllers[i].vibrationTimeRight = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////
//Destructor
//////////////////////////////////////////////////////////////////////////////
Input::~Input(void)
{
	if(mouseCaptured) ReleaseCapture();	//Release mouse
}

//Public function
//««««««««««

//////////////////////////////////////////////////////////////////////////////
//Initialize mouse and controller input
//If you want to capture mouse, capture = true
//throw GameError
//////////////////////////////////////////////////////////////////////////////
void Input::initialize(HWND hwnd, bool capture)
{
	try
	{
		mouseCaptured = capture;

		//Register high-definition mouse
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = RIDEV_INPUTSINK;
		Rid[0].hwndTarget = hwnd;

		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

		if(mouseCaptured)
		{
			SetCapture(hwnd);	//Capture mouse
		}

		//Clear controller
		ZeroMemory(controllers, sizeof(ControllerState) * MAX_CONTROLLERS);
		checkControllers();	//Check connected controller
	}
	catch(...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialize input system"));
	}
}

//////////////////////////////////////////////////////////////////////////////
//Set true keysDown array and keysPressed array corresponding the specified key
//Before run : wParam store virtual key code (0 ~ 255)
//////////////////////////////////////////////////////////////////////////////
void Input::keyDown(WPARAM wParam)
{
	//Check key code is more than 0 and less than 255
	if(wParam <inputNS::KEYS_ARRAY_LEN)
	{
		//If the key was pressed already, else keysDown array was deleted with clear()
		if(keysDown[wParam]) keysPressed[wParam] = false;	//Update keysPressed array
		else keysPressed[wParam] = true;

		keysDown[wParam] = true;		//Update keysDown array

		//If the key was pressed already, else keysDown array was deleted with clear()
		//keysPressed[wParam] = true;	//Update keysPressed array
	}
}

//////////////////////////////////////////////////////////////////////////////
//Set false keysDown array and keysPressed array corresponding the specified key
//Before run : wParam store virtual key code (0 ~ 255)
//////////////////////////////////////////////////////////////////////////////
void Input::keyUp(WPARAM wParam)
{
	//Check key code is more than 0 and less than 255
	if(wParam <inputNS::KEYS_ARRAY_LEN)
	{
		//Update state table
		keysDown[wParam] = false;
	}
}

//////////////////////////////////////////////////////////////////////////////
//Store input charactor in textIn array
//Before run : wParam store charactor
//////////////////////////////////////////////////////////////////////////////
void Input::keyIn(WPARAM wParam)
{
	if(newLine)	//Start new line
	{
		textIn.clear();
		newLine = false;
	}
	
	if(wParam == '\b')	//If input backspace
	{
		//If Stored charactor in textIn
		if(textIn.length() > 0) textIn.erase(textIn.size() - 1);	//Delete last charactor
	}
	else
	{
		textIn += wParam;
		charIn += wParam;
	}

	if((char)wParam == '\r') newLine = true;
}

//////////////////////////////////////////////////////////////////////////////
//If specified virtual key is pressed, return true, else return false
//////////////////////////////////////////////////////////////////////////////
bool Input::isKeyDown(UCHAR vkey) const
{
	if(vkey < inputNS::KEYS_ARRAY_LEN) return keysDown[vkey];
	else return false;
}


//ªªªªªªªªªª