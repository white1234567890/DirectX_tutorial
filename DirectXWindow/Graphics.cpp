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

//Private function
//««««««««««

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

//ªªªªªªªªªª

//Public function
//««««««««««

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

	//If fullscreen mode
	if(fullscreen)
	{
		//Check Adapter is compatible
		if(isAdapterCompatible())
		{
			//Set adapter reflesh rate
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		}
		else
		{
			throw(GameError(gameErrorNS::FATAL_ERROR, "The graphics device does not support the specified resolution and/or format"));
		}
	}

	//Check graphics card support hardware texturing, lighting, and vertex shader
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	//If device does not support HWtexturing and lighting, or 1.1 vertex shader, switch software vertex processing
	if((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1,1))
	{
		//Process software only
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		//Process hardware only
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	//Create Direct3D device
	result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, behavior, &d3dpp, &device3d);
	if(FAILED(result))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creatind Direct3D device"));
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

///////////////////////////////////////////////////////////////////////////////////////////////////
//Check adapter and, check BackBuffer height, width and refresh rate specified by d3dpp supported
//If supported adapter was discovered, set proper format to pMode
//Before run	: d3dpp was initialized
//After run		: If discoverd proper format and set it to pMode, return true, else return false
///////////////////////////////////////////////////////////////////////////////////////////////////
bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat);
	for (unsigned int i = 0; i < modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &pMode);

		if(pMode.Height == d3dpp.BackBufferHeight && pMode.Width == d3dpp.BackBufferWidth && pMode.RefreshRate == d3dpp.FullScreen_RefreshRateInHz) return true;
	}

	return false;
}

//ªªªªªªªªªª