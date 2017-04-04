//You Oyadomari
//Kokusai Denshi Business College
//Initial 2017/2/17
//LastUpdate 2017/2/17

#include "ReCreation.h"


ReCreation::ReCreation(void)
{
}


ReCreation::~ReCreation(void)
{
	//Call onLostDevice() about all graphics items
	releaseAll();
}

//////////////////////////////////////////////////////////////////////////////
//Initialize game
//If error, throw GameError
//////////////////////////////////////////////////////////////////////////////
void ReCreation::initialize(HWND hwnd)
{
	Game::initialize(hwnd);	//Throw game error

	//texture of nebula
	if(!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));
	}
	
	//texture of planet
	if(!planetTexture.initialize(graphics, PLANET_IMAGE))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializeing planet texture"));
	}

	//nebula
	if(!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	}

	//planet
	if(!planet.initialize(graphics, 0, 0, 0, &planetTexture))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
	}

	//set planet in center of screen
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	return;
}

//////////////////////////////////////////////////////////////////////////////
//Update all game items
//////////////////////////////////////////////////////////////////////////////
void ReCreation::update()
{
}

//////////////////////////////////////////////////////////////////////////////
//Artificial intelligence
//////////////////////////////////////////////////////////////////////////////
void ReCreation::ai()
{
}

//////////////////////////////////////////////////////////////////////////////
//Work collisions
//////////////////////////////////////////////////////////////////////////////
void ReCreation::collisions()
{
}

//////////////////////////////////////////////////////////////////////////////
//Render game items
//////////////////////////////////////////////////////////////////////////////
void ReCreation::render()
{
	graphics->spriteBegin();	//begin draw sprite

	nebula.draw();	//add orion in the scene
	planet.draw();	//add planet int the scene

	graphics->spriteEnd();	//end draw sprite
}

//////////////////////////////////////////////////////////////////////////////
//If graphics devices lost, release all video memories to be able to reset graphics devices
//////////////////////////////////////////////////////////////////////////////
void ReCreation::releaseAll()
{
	planetTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	Game::releaseAll();
	return;
}

//////////////////////////////////////////////////////////////////////////////
//If graphics devices are reset, remake all surfaces
//////////////////////////////////////////////////////////////////////////////
void ReCreation::resetAll()
{
	nebulaTexture.onResetDevice();
	planetTexture.onResetDevice();
	Game::resetAll();
	return;
}