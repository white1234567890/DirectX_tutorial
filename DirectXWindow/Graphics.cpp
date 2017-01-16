//Author You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/11
//Last update 2017/1/11

#include "Graphics.h"

//////////////////////////////////////////////////////////////////////////////
//Constructor
//////////////////////////////////////////////////////////////////////////////
Graphics::Graphics(void)
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	//Width and height can replace initialize()
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
}

//////////////////////////////////////////////////////////////////////////////
//Destructor
//////////////////////////////////////////////////////////////////////////////
Graphics::~Graphics(void)
{
	releaseAll();
}

//////////////////////////////////////////////////////////////////////////////
//Release all	
//////////////////////////////////////////////////////////////////////////////
void Graphics::releaseAll()
{
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

//////////////////////////////////////////////////////////////////////////////
//Initialize DirectX graphics
//When detect error, throw GameError
//////////////////////////////////////////////////////////////////////////////
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;
	
	//Initialize Direct3D
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(direct3d == NULL)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));
	}

	//Initialize D3DPresentationParameter
	initD3Dpp();

	//Create Direct3D device
	result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,//If graphics card don't support hardware vertex processing, use "D3DCREATE_HARDWARE_VERTEXPRICESSING"
		&d3dpp, &device3d);
	if(FAILED(result))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creatind Direct3D device"));
	}
}

//////////////////////////////////////////////////////////////////////////////
//Initialize D3DPresentatoinParameter
//////////////////////////////////////////////////////////////////////////////
void Graphics::initD3Dpp()
{
	try
	{
		//Set 0 in struct
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		//Set necessary parameter
		//««««««««««
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		
		//Process full screen
		if(fullscreen)
		{
			//Set 24 bit color
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		}
		else
		{
			//Use desctop configuration
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		}
		
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		//ªªªªªªªªªª
	}
	catch(...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing D3D presentation parameters"));
	}
}

HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;//Default is error. Replace when success

	//"This function move next version"
	//Clear back buffer to rime green
	device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,255,0), 0.0f, 0);

	//Draw back buffer on display
	result = device3d->Present(NULL, NULL, NULL, NULL);
	
	return result;
}