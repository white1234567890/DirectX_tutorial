//You Oyadomari
//Kokusai Denshi Business Colledge
//Initial 2017/4/4
//LastUpdate 2017/4/4

#include "Image.h"

//////////////////////////////////////////////////////////////////////////////
//default constructor
//////////////////////////////////////////////////////////////////////////////
Image::Image(void)
{
	initialized = false;
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0f;
	spriteData.y = 0.0f;
	spriteData.scale = 1.0f;
	spriteData.angle = 0.0f;
	spriteData.rect.left = 0;
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0f;
	animTimer = 0.0f;
	visible = true;
	loop = true;
	animComplete = false;
	graphics =NULL;
	colorFilter = graphicsNS::WHITE;
}

//////////////////////////////////////////////////////////////////////////////
//destructor
//////////////////////////////////////////////////////////////////////////////
Image::~Image(void)
{
}

//////////////////////////////////////////////////////////////////////////////
// Initialize the Image.
// Post: returns true if successful, false if failed
// pointer to Graphics
// width of Image in pixels  (0 = use full texture width)
// height of Image in pixels (0 = use full texture height)
// number of columns in texture (1 to n) (0 same as 1)
// pointer to TextureManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Image::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM)
{
	try{
		graphics = g;						//graphics object
		textureManager = textureM;	//pointer to texture object
		spriteData.texture = textureManager->getTexture();

		if(width == 0)
		{
			width = textureManager->getWidth();	//use texture's width
		}
		spriteData.width = width;

		if(height == 0)
		{
			height = textureManager->getWidth();	//use texture's height
		}
		spriteData.height = height;

		
		cols = ncols;
		if(cols == 0)
		{
			cols = 1;	//if cols == 0, use 1
		}

		//configure spriteData.rect to draw currentFrame
		//this code is used when select a part of texture
		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		//right edge + 1
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame / cols) * spriteData.height;
		//bottom edge + 1
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}catch(...) {return false;}

	initialized = true;	//successfully initialized
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//draw image to use color as filter
//color is option(default color is defined white in image.h)
//pre:	call spriteBegin()
//post:	call spriteEnd()
//////////////////////////////////////////////////////////////////////////////
void Image::draw(COLOR_ARGB color)
{
	if(!visible || graphics == NULL) return;
	
	//get new texture when call onReset()
	spriteData.texture = textureManager->getTexture();
	
	//if draw by color filter
	if(color == graphicsNS::FILTER)
	{
		//use colorFilter
		graphics->drawSprite(spriteData, colorFilter);
	}
	else
	{
		//use color as a filter
		graphics->drawSprite(spriteData, color);
	}
}

//////////////////////////////////////////////////////////////////////////////
//draw image use specified SpriteData
//select the texture using now SpriteData.rect
//pre:	call spriteBegin()
//post:	call spriteEnd()
//////////////////////////////////////////////////////////////////////////////
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
	if(!visible || graphics == NULL) return;

	//select texture use this imege's rect
	sd.rect = spriteData.rect;
	//get new texture when call onReset()
	sd.texture = textureManager->getTexture();
	
	//if use color filter
	if(color == graphicsNS::FILTER)
	{
		//use colorFilter
		graphics->drawSprite(sd, colorFilter);
	}
	//if use color as filter
	else
	{
		//use color
		graphics->drawSprite(sd, color);
	}
}

//////////////////////////////////////////////////////////////////////////////
//update 
//typically call once per frame
//frameTime: used to regulate the speed of movement and animation
//////////////////////////////////////////////////////////////////////////////
void Image::update(float frameTime)
{
	if(endFrame - startFrame > 0)	//if animated sprite
	{
		animTimer += frameTime;	//total elapsed time
		if(animTimer > frameDelay)
		{
			animTimer -= frameDelay;
			currentFrame++;
			if(currentFrame < startFrame || currentFrame > endFrame)
			{
				if(loop == true)	//if looping animation
				{
					currentFrame = startFrame;
				}
				else	//not looping animation
				{
					currentFrame = endFrame;
					animComplete = true;	//animation complete
				}
			}
			setRect();	//set spriteData.rect
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//set the current frame of the image
//use when change animation sequence or select a texture from image that have some texrures
//c:	set current frame
//////////////////////////////////////////////////////////////////////////////
void Image::setCurrentFrame(int c)
{
	if(c >= 0)
	{
		currentFrame = c;
		animComplete = false;
		setRect();	//set spriteData.rect
	}
}

//////////////////////////////////////////////////////////////////////////////
//set spriteData.rect to draw currentFrame
//////////////////////////////////////////////////////////////////////////////
inline void Image::setRect()
{
	//configure spriteData.rect to draw currentFrame
	//Set rect a part of texture using currentFrame
	//and draw image in the rect
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	//right edge + 1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / cols) * spriteData.height;
	//bottom edge + 1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}