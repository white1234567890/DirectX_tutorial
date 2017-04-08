//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/4/5
//LastUpdate 2017/4/7

#pragma once
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace planetNS
{
	const int WIDTH = 128;							//image width
	const int HEIGHT = 128;						//image height
	const int COLLISION_RADIUS = 120/2;	//for circular collision
	const int X = GAME_WIDTH / 2 - WIDTH / 2;	//location on screen
	const int Y = GAME_HEIGHT / 2 - WIDTH / 2;
	const float MASS = 1.0e14f;					//mass
	const int TEXTURE_COLS = 2;				//texture has 2 columns
	const int START_FRAME = 1;					//starts at frame 1
	const int END_FRAME = 1;						//no animation
}

//inherits from Entity class
class Planet :
	public Entity
{
public:
	// constructor
	Planet(void);
};

