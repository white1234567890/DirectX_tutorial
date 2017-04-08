//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/4/5
//LastUpdate 2017/4/5

#pragma once
#define WIN_32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace playerNS
{
	const int WIDTH = 32;			//image width
	const int HEIGHT = 32;			//image height
	const int X = GAME_WIDTH / 2 - WIDTH / 2;	//location on screen
	const int Y = GAME_HEIGHT / 2 * HEIGHT / 2;
	const float ROTATION_RATE = (float)PI / 4;	//radians per second
	const float SPEED = 100.0f;		//100 pixels per second
	const float MASS = 300.0f;		//mass
	const int TEXTURE_COLS = 2;	//texture has 2 columns
	const int PLAYER_START_FRAME = 0;	//player starts at frame 0
	const int PLAYER_END_FRAME = 3;		//player animation frames 0,1,2,3
	const float PLAYER_ANIMATION_DELAY = 0.1f;	//time between frames
}

//inherits from Entity class
class Player :
	public Entity
{
private:
	bool shieldOn;
	Image shield;

public:
	//constructor
	Player(void);

	//inherited mamber function
	virtual void draw();
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM); 
	void update(float frameTime);
	void damage(WEAPON);
};

