//You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/4/4
//LastUpdate 2017/4/4

#pragma once
#define WIN32_LEAN_AND_MEAN

#include "TextureManager.h"
#include "constants.h"

//all code that manage drawing sprite must written in this class
class Image
{
	//Image properties
protected:
	Graphics *graphics;						//pointer to graphics
	TextureManager *textureManager;	//pointer to texture manager
	
	//spriteData contains the data required to draw the image by Graphics::drawSprite()
	SpriteData spriteData;	//SpriteData is defined in "Graphics.h"
	COLOR_ARGB colorFilter;	//applied as a color filter (use WHITE for no change)
	int cols;						//number of cols(1 to n) in multi-frame sprite
	int startFrame;				//first frame of current animation
	int endFrame;					//end frame of current animation
	int currentFrame;			//current frame of animation
	float frameDelay;			//how long between frames of animation
	float animTimer;				//animation timer
	HRESULT hr;					//standard return type
	bool loop;						//true to loop frames
	bool visible;					//true when visible
	bool initialized;			//true when successfully initialized
	bool animComplete;			//true when loop is false and endFrame has finished displaying

public:
	//constructor
	Image(void);
	//destructor
	virtual ~Image(void);

	//Getter
	//««««««««««

	//return reference to SpriteData structure
	const virtual SpriteData &getSpriteInfo() {return spriteData;}

	//return visible parameter
	virtual bool getVisible() {return visible;}

	//return x position
	virtual float getX() {return spriteData.x;}

	//return y position
	virtual float getY() {return spriteData.y;}

	//return scale factor
	virtual float getScale() {return spriteData.scale;}

	//return width
	virtual int getWidth() {return spriteData.width;}

	//return height
	virtual int getHeight() {return spriteData.height;}

	//return center x
	virtual float getCenterX() {return spriteData.x + spriteData.width / 2 * spriteData.scale;}

	//return center y
	virtual float getCenterY() {return spriteData.y + spriteData.height / 2 * spriteData.scale;}

	//return rotation angle in degrees
	virtual float getDegrees() {return spriteData.angle * (180.0f / (float)PI);}

	//return rotation angle in radians
	virtual float getRadians() {return spriteData.angle;}

	//return delay between frames of animation
	virtual float getFrameDelay() {return frameDelay;}

	//return number of starting frame
	virtual int getStartFrame() {return startFrame;}

	//return number of ending frame
	virtual int getEndFrame() {return endFrame;}

	//return number of current frame
	virtual int getCurrentFramw() {return currentFrame;}

	//return RECT structure of Image
	virtual RECT getSpriteDataRect() {return spriteData.rect;}

	//return state of animation complete
	virtual bool getAnimationComplete() {return animComplete;}

	//return colorFilter
	virtual COLOR_ARGB getColorFilter() {return colorFilter;}

	//ªªªªªªªªªª

	//Setter
	//««««««««««

	//set x location
	virtual void setX(float newX) {spriteData.x = newX;}

	//set y location 
	virtual void setY(float newY) {spriteData.y = newY;}

	//set scale
	virtual void setScale(float s) {spriteData.scale = s;}

	//set rotation angle in degrees
	//0 degrees is up
	//angles progress clockwise
	virtual void setDegree(float deg) {spriteData.angle = deg * ((float)PI / 180.0f);}

	//set rotation angle in radians
	//0 radians is up
	//angle progress clockwise
	virtual void setRadians(float rad) {spriteData.angle = rad;}
	
	//set visible
	virtual void setVisible(bool v) {visible = v;}

	//set delay between frame of animation
	virtual void setFrameDelay(float d) {frameDelay = d;}

	//set starting and ending frame of animation
	virtual void setFrames(int s, int e) {startFrame = s; endFrame = e;}
	
	//set current frame of animaion
	virtual void setCurrentFrame(int c);

	//set spriteData.rect to draw currentFrame
	virtual void setRect();

	//set spriteData.rect to r
	virtual void setSpriteDataRect(RECT r) {spriteData.rect = r;}

	//set animation loop
	//lp = true to loop
	virtual void setLoop(bool lp) {loop = lp;}

	//set animation complete Boolean
	virtual void setAnimationComplete(bool a) {animComplete = a;}

	//set color filter (use WHITE for no change)
	virtual void setColorFilter(COLOR_ARGB color) {colorFilter = color;}

	//set texture manager
	virtual void setTextureManager(TextureManager *textureM) {textureManager = textureM;}

	//ªªªªªªªªªª

	//Initialize Image
	//*g :			pointer to Graphics object
	//width:			width of Image in pixels (0 = use full texture width)
	//height:		height of Image in pixels (0 = use full texture height)
	//ncols:			number of columns in texture (1 to n) {0 same as 1)
	//*textureM:	pointer to TextureManager object
	virtual bool initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM);

	//Flip image horuzontally (mirror)
	virtual void flipHorizontal(bool flip) {spriteData.flipHorizontal = flip;}

	//Flip image vertically
	virtual void flipVertical(bool flip) {spriteData.flipVertical = flip;}

	//draw Image using color as filter
	//default color is WHITE
	virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);

	//draw this image using the specified SpriteData
	//The current SpriteData.rect is used to select the texture
	//sd:		SpriteData
	//color:	color filter
	virtual void draw(SpriteData sd, COLOR_ARGB color);

	//Update the animation
	//frameTime is used to regulate the speed
	virtual void update(float frameTime);
};

