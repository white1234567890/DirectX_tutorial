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
#define LP_TEXTURE LPDIRECT3DTEXTURE9	//Redefine texture pointer

//Define color
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) ((COLOR_ARGB)((((a)&0xff)<<24) | (((r)&0xff)<<16) | (((g)&0xff)<<8) | ((b)&0xff)))

//Define vector2
#define VECTOR2 D3DXVECTOR2

namespace graphicsNS
{
	// Some common colors
    // ARGB numbers range from 0 through 255
    // A = Alpha channel (transparency where 255 is opaque)
    // R = Red, G = Green, B = Blue
    const COLOR_ARGB ORANGE  = D3DCOLOR_ARGB(255,255,165,  0);
    const COLOR_ARGB BROWN   = D3DCOLOR_ARGB(255,139, 69, 19);
    const COLOR_ARGB LTGRAY  = D3DCOLOR_ARGB(255,192,192,192);
    const COLOR_ARGB GRAY    = D3DCOLOR_ARGB(255,128,128,128);
    const COLOR_ARGB OLIVE   = D3DCOLOR_ARGB(255,128,128,  0);
    const COLOR_ARGB PURPLE  = D3DCOLOR_ARGB(255,128,  0,128);
    const COLOR_ARGB MAROON  = D3DCOLOR_ARGB(255,128,  0,  0);
    const COLOR_ARGB TEAL    = D3DCOLOR_ARGB(255,  0,128,128);
    const COLOR_ARGB GREEN   = D3DCOLOR_ARGB(255,  0,128,  0);
    const COLOR_ARGB NAVY    = D3DCOLOR_ARGB(255,  0,  0,128);
    const COLOR_ARGB WHITE   = D3DCOLOR_ARGB(255,255,255,255);
    const COLOR_ARGB YELLOW  = D3DCOLOR_ARGB(255,255,255,  0);
    const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255,255,  0,255);
    const COLOR_ARGB RED     = D3DCOLOR_ARGB(255,255,  0,  0);
    const COLOR_ARGB CYAN    = D3DCOLOR_ARGB(255,  0,255,255);
    const COLOR_ARGB LIME    = D3DCOLOR_ARGB(255,  0,255,  0);
    const COLOR_ARGB BLUE    = D3DCOLOR_ARGB(255,  0,  0,255);
    const COLOR_ARGB BLACK   = D3DCOLOR_ARGB(255,  0,  0,  0);
    const COLOR_ARGB FILTER  = D3DCOLOR_ARGB(  0,  0,  0,  0);  // use to specify drawing with colorFilter
    const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB( 64,255,255,255);  // AND with color to get 25% alpha
    const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128,255,255,255);  // AND with color to get 50% alpha
    const COLOR_ARGB BACK_COLOR = NAVY;                         // background color of game

    enum DISPLAY_MODE{TOGGLE, FULLSCREEN, WINDOW};
}

//SpraiteData
struct SpriteData
{
	int width;					//width(pixels)
	int height;				//height(pixels)
	float x,y;					//Screen location (top left corer of sprite)
	float scale;				//less then 1 is reduceing, more then 1 is expanding
	float angle;				//rotation angle (radian)
	RECT rect;					//used to select an image from a larger texture
	LP_TEXTURE texture;	//pointer to texture
	bool flipHorizontal;	//true to flip sprite horizontally
	bool flipVertical;		//true to flip sprite vertically
};

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
	IDirect3DQuery9* pOcclusionQuery;	//for pixel perfect collision detection
	DWORD numberOfPixelsColliding;		//for pixel perfect collision detection

	//Variavles
	HRESULT result;				//Windows default return code
	HWND hwnd;
	bool fullscreen;
	bool stencilSupport;		//true if device supports stencil buffer
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

	//Getter
	//��������������������

	//return  direct3d
	LP_3D get3D() {return direct3d;}

	//return device3d
	LP_3DDEVICE get3Ddevice() {return device3d;}

	//return sprite
	LP_SPRITE getSprite() {return sprite;}

	//return handle to device context(window)
	HDC getDC() {return GetDC(hwnd);}

	//return fullscreen
	bool getFullscreen() {return fullscreen;}

	//return pOcclusionQuery
	IDirect3DQuery9* detPOcclusionQuery() {return pOcclusionQuery;}

	//return true if the graphics card supports a stencil buffer
	bool getStencilSupport() {return stencilSupport;}

	//��������������������

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

	//Draw SpriteData struct
	//color : applied as a filter, White is default
	//Creates a sprite Begin/End pair
	//Before run : call sprite->Begin()
	//After run :	call sprite->End()
	//					spriteData.rect defines the portion of spriteData.texture
	//					spriteData.rect.right must be right edge + 1
	//					spriteData.rect.bottom must be bottom edge + 1
	void drawSprite(const SpriteData &spriteData, COLOR_ARGB color = graphicsNS::WHITE);

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

	//sprite begin
	void spriteBegin()
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
	//sprite end
	void spriteEnd()
	{
		sprite->End();
	}

	//Load texture to defalt D3D memory
	//this function use in engine only
	//*filename : name of load file
	//transcolor : ARGB
	//&width : width
	//&height : height
	//&texture : type of texture
	HRESULT loadTexture(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);

	//Load texture to system memory (system memory can lock)
	//this function be able to access directry to pixels data
	//*filename : name of load file
	//transcolor : ARGB
	//&width : width
	//&height : height
	//&texture : type of texture
	HRESULT loadTextureSystemMem(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);

	//return length of vector v
	static float Vector2Length(const VECTOR2 *v) {return D3DXVec2Length(v);}

	//return Dot product of v1 and v2
	static float Vector2Dot(const VECTOR2 *v1, const VECTOR2 *v2) {return D3DXVec2Dot(v1, v2);}

	//normalize vector v
	static void Vector2Normalize(VECTOR2 *v) {D3DXVec2Normalize(v, v);}

	//transform vector v with matrix m
	static VECTOR2* Vector2Transform(VECTOR2* v, D3DXMATRIX *m) {return D3DXVec2TransformCoord(v, v, m);}

	//return the number oc pixels colliding between the two sprites
	//pre:	the device supprots a stencil buffer and pOcclunsionQuery points to a valid occlusionQuery object
	//post:	return the number of pixels of overlap
	DWORD pixelCollision(const SpriteData &sprite1,	const SpriteData &sprite2);
};

