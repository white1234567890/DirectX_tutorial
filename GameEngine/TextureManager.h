//You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/4/4
//LastUpdate 2017/4/4

#pragma once
#define WIN32_LEAN_AND_MEAN

#include "Graphics.h"
#include "constants.h"


class TextureManager
{
	//TextureManager properties
private:
	UINT width;				//width of texture(pixels)
	UINT height;				//height of texture(pixels)
	LP_TEXTURE texture;	//pointer to texture
	const char *file;		//name of file
	Graphics *graphics;	//save pointer to graphics
	bool initialized;		//true when successfully initialized
	HRESULT hr;				//standard return type

public:
	//constructor
	TextureManager(void);
	//destructor
	virtual ~TextureManager(void);

	//returns a pointer to the texture
	LP_TEXTURE getTexture() const {return texture;}

	//return the texture width
	UINT getWidth() const {return width;}

	//return the texture height
	UINT getHeight() const {return height;}

	//Initialize the texruteManader
	//*g : points to Graphics object
	//*f : points to name of texture file to load
	//post:	the texture file is loaded
	virtual bool initialize(Graphics *g, const char *f);

	//release resources
	virtual void onLostDevice();

	//restore resources
	virtual void onResetDevice();
};

