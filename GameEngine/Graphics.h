#pragma once
//Author You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/11
//Last update 2017/2/20

#define WIN32_LEAN_AND_MEAN
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include "constants.h"
#include "gameError.h"

//DirectX pointer type
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D LPDIRECT3D9
#define LP_SPRITE LPD3DXSPRITE	//Redefine sprite pointer

//Define color
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) ((COLOR_ARGB)((((a)&0xff) << 24 | (((r)&0xff) << 16 | (((g)&0xff << 8 | ((b)&0xff)))

//This class is rapper class of DirectX graphics code
class Graphics
{
private:
	//DirectX pointer
	LP_3D direct3d;
	LP_3DDEVICE device3d;
	LP_SPRITE sprite;	//All sprite ude this pointer
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	//Variavles
	HRESULT result;				//Windows default return code
	HWND hwnd;
	bool fullscreen;
	int width;
	int height;
	COLOR_ARGB backColor;	//Background color

	//Use in engine only. Nothing user use function
	
	//Initialize D3DPresentationParamater
	void initD3Dpp();

public:
	//Default constructor
	Graphics(void);
	
	//Destructor
	~Graphics(void);

	//Release direct3d and device3d to use SAFE_RELEASE macro
	void releaseAll();

	//Initialize Direct3D, and create Direct3D device
	//hw : Handle to window
	//width : width(pixel)
	//height : height(pixel)
	//fullscreen : IF use fullscreen, it is true, else is false
	//When detect error, throw GameError
	void initialize(HWND hw, int width, int height, bool fullscreen);

	//Draw off screen back buffer on display using page flipping
	HRESULT showBackbuffer();

	//Check adapter and, check BackBuffer height, width and refresh rate specified by d3dpp supported
	//If supported adapter was discovered, set proper format to pMode
	//Before run	: d3dpp was initialized
	//After run	: If discoverd proper format and set it to pMode, return true, else return false
	bool isAdapterCompatible();

	//Check device was lost
	//Call TestCooperatativeLevel function
	//If return D3D_OK, device run normally, else
	//D3DERR_DEVICELOST : device was lost, and can not restore now
	//D3DERR_DEVICENOTRESET : can reset device
	//D3DERR_DRIVERINTERNALERROR : device send message of internal error, and there is nothing other than to report error to user
	//It is necessary that reinitialize D3D presentation paramerer to reset graphic device
	//This function need calling before run reset function
	HRESULT getDeviceState();

	//If Direcr3D graphics device was lost, DirectX window contents can not draw
	//Direct3D device was disabled rendering state, somtimes it was lost from program
	//If Devoce was lost, must reset device, and recreate resource
	//FLOW
	//If device is disabled
	//		If device was lost and can not reset
	//			Wait still device is restore
	//		If device was lost and can reset
	//			Try to reset Device
	//			If reset fail
	//				return
	//reset function is only method to recall lost device
	//If all resources that are assigned by D3DPOOL_DEFAULT are not released, reset function fail 
	//Resetting graphics device needs to call this function after reinitialize D3D presentation parameter
	HRESULT reset();

	//DirectX has some graphic figures renderable
	//These figures are called primitive
	//Drawing primitive figures is called rendering
	//First step of rendering DirectX's primitive is calling BeginScene function of DirectX
	//After finish all rendering, exit scene calling EndScene function
	//Out of scene primitive rendering will fail
	//This function clear all back buffer, and call BeginScene() function of DirectX
	HRESULT beginScene()
	{
		result = E_FAIL;	//Default is fail

		//Device is not, return fail
		if(device3d == NULL) return result;

		//Clear back buffer by backColor
		device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0F, 0);

		//Start scene to render
		result = device3d->BeginScene();
		return result;
	}

	//All DirectX's primitives rendering need processing in scene
	//Call EndScene() function of DirectX
	HRESULT endScene()
	{
		result = E_FAIL;	//Default is fail

		if(device3d) result = device3d->EndScene();
		return result;
	}
};

