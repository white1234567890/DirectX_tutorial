//You Oyadomari
//Kokusai Denshi Business College
//Initial 2017/2/17
//LastUpdate 2017/4/5

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

	//player texture
	if(!playerTexture.initialize(graphics, PLAYER_IMAGE))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player texture"));
	}

	//player image
	if(!player.initialize(this, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_COL, &playerTexture))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player"));
	}

	//start left top
	player.setX((float)GAME_WIDTH / 4);
	player.setY((float)GAME_HEIGHT / 4);

	//set start and end frame of player animation
	player.setFrames(PLAYER_START_FRAME, PLAYER_END_FRAME);	//animation frame
	player.setCurrentFrame(PLAYER_START_FRAME);	//set start frame to current frame
	player.setFrameDelay(PLAYER_ANIMATION_DELAY);	//set player animation delay
	player.setDegree(45.0f);	//player angle;
	player.setVelocity(VECTOR2(playerNS::SPEED, -playerNS::SPEED));	//velocity

	return;
}

//////////////////////////////////////////////////////////////////////////////
//Update all game items
//////////////////////////////////////////////////////////////////////////////
void ReCreation::update()
{
	////move right
	//if(input->isKeyDown(PLAYER_RIGHT_KEY))
	//{
	//	player.setX(player.getX() + frameTime * PLAYER_SPEED);
	//	if(player.getX() > GAME_WIDTH - player.getWidth()) //if over right edge
	//	{
	//		//set location on right edge
	//		player.setX((float)(GAME_WIDTH - player.getWidth()));
	//	}
	//}

	////move left
	//if(input->isKeyDown(PLAYER_LEFT_KEY))
	//{
	//	player.setX(player.getX() - frameTime * PLAYER_SPEED);
	//	if(player.getX() < player.getWidth())	//if over left edge
	//	{
	//		//set location on left edge
	//		player.setX((float)player.getWidth());
	//	}
	//}

	////move up
	//if(input->isKeyDown(PLAYER_UP_KEY))
	//{
	//	player.setY(player.getY() - frameTime * PLAYER_SPEED);
	//	if(player.getY() < player.getHeight())	//if over top edge
	//	{
	//		//set location on top edge
	//		player.setY((float)player.getHeight());
	//	}
	//}

	////move down
	//if(input->isKeyDown(PLAYER_DOWN_KEY))
	//{
	//	player.setY(player.getY() + frameTime * PLAYER_SPEED);
	//	if(player.getY() > GAME_WIDTH - player.getHeight())	//if over bottom edge
	//	{
	//		//set location on bottom edge
	//		player.setY((float)(GAME_WIDTH - player.getHeight()));
	//	}
	//}

	player.update(frameTime);
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
	VECTOR2 collisionsVector;
}

//////////////////////////////////////////////////////////////////////////////
//Render game items
//////////////////////////////////////////////////////////////////////////////
void ReCreation::render()
{
	graphics->spriteBegin();	//begin draw sprite

	player.draw();	//add player in the scene

	graphics->spriteEnd();	//end draw sprite
}

//////////////////////////////////////////////////////////////////////////////
//If graphics devices lost, release all video memories to be able to reset graphics devices
//////////////////////////////////////////////////////////////////////////////
void ReCreation::releaseAll()
{
	playerTexture.onLostDevice();
	Game::releaseAll();
	return;
}

//////////////////////////////////////////////////////////////////////////////
//If graphics devices are reset, remake all surfaces
//////////////////////////////////////////////////////////////////////////////
void ReCreation::resetAll()
{
	playerTexture.onResetDevice();
	Game::resetAll();
	return;
}