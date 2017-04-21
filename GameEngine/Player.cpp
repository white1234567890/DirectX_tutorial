#include "Player.h"

//////////////////////////////////////////////////////////////////////////////
//default constructor
//////////////////////////////////////////////////////////////////////////////
Player::Player(void) : Entity()
{
	spriteData.width = playerNS::WIDTH;				//size of player
	spriteData.height = playerNS::HEIGHT;
	spriteData.x = (float)playerNS::X;							//location on screen
	spriteData.y = (float)playerNS::Y;
	spriteData.rect.bottom = playerNS::HEIGHT;	//rectangle to select parts of an image
	spriteData.rect.right = playerNS::WIDTH;
	velocity.x = 0;											//velocity X
	velocity.y = 0;											//velocity Y
	frameDelay = playerNS::PLAYER_ANIMATION_DELAY;
	startFrame = playerNS::PLAYER_START_FRAME;		//first frame of player animation
	endFrame = playerNS::PLAYER_END_FRAME;		//last framw of player animation
	currentFrame = startFrame;
	radius = playerNS::WIDTH / 2.0f;
	collisionType = entityNS::CIRCLE;
}

//////////////////////////////////////////////////////////////////////////////
//uodate
//typically called once per frame
//frameTime:	used to ragulate the speed pf movement
//////////////////////////////////////////////////////////////////////////////
void Player::update(float frameTime)
{
	Entity::update(frameTime);
	spriteData.angle += frameTime * playerNS::ROTATION_RATE; //rotate player

	spriteData.x += frameTime * velocity.x;	//move player to X axis
	spriteData.y += frameTime * velocity.y;	//move player to Y axis

	velocity.y = frameTime * GRAVITY;	//drap down follow gravity

	//bound on wall
	//if collide on right edge
	if(spriteData.x > GAME_WIDTH - playerNS::WIDTH * getScale())
	{
		//set right edge
		spriteData.x = GAME_WIDTH - playerNS::WIDTH * getScale();
		velocity.x = -velocity.x;		//reflect velocity
	}
	//if collide on left edge
	else if(spriteData.x < 0)
	{
		//set left edge
		spriteData.x = 0;
		velocity.x = -velocity.x;
	}

	//if collide on bottom edge
	if(spriteData.y > GAME_HEIGHT - playerNS::HEIGHT * getScale())
	{
		//set bottom edge
		spriteData.y = GAME_HEIGHT - playerNS::HEIGHT * getScale();
		velocity.y = -velocity.y;
	}
	//if collide on top edge
	if(spriteData.y < 0)
	{
		//set top edge
		spriteData.y = 0;
		velocity.y = -velocity.y;
	}
}