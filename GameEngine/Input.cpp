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