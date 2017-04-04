//You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/4/4
//LastUpdate 2017/4/4

#include "TextureManager.h"

//////////////////////////////////////////////////////////////////////////////
//default constructor
//////////////////////////////////////////////////////////////////////////////
TextureManager::TextureManager(void)
{
	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;
}

//////////////////////////////////////////////////////////////////////////////
//destructor
//////////////////////////////////////////////////////////////////////////////
TextureManager::~TextureManager(void)
{
	SAFE_RELEASE(texture);
}

//public function
//««««««««««

//////////////////////////////////////////////////////////////////////////////
//load texture file from disk
//post:	return true if successful, false if failed
//////////////////////////////////////////////////////////////////////////////
bool TextureManager::initialize(Graphics *g, const char *f)
{
	try{
		graphics = g;	//grapics object
		file = f;		//texture file
		hr = graphics->loadTexture(file, TRANSCOLOR, width, height, texture);

		if(FAILED(hr))
		{
			SAFE_RELEASE(texture);
			return false;
		}
	}catch(...) {return false;}
	
	initialized = true;	//if successfully initialized, set true
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//call when graphics device was lost
//////////////////////////////////////////////////////////////////////////////
void TextureManager::onLostDevice()
{
    if (!initialized) return;
    SAFE_RELEASE(texture);
}

//////////////////////////////////////////////////////////////////////////////
//call when graphics device was reset
//////////////////////////////////////////////////////////////////////////////
void TextureManager::onResetDevice()
{
    if (!initialized) return;
    graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}

//ªªªªªªªªªª