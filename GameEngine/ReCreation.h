//You Oyadomari
//Kokusai Denshi Business College
//Initial 2017/2/17

//LastUpdate 2017/2/17
#pragma once
#define WIN32_LEAN_AND_MEAN
#include "game.h"
#include "TextureManager.h"
#include "Image.h"

//////////////////////////////////////////////////////////////////////////////
//Game entity class
//inherit : Game class
//update(), ai(), collosions(), render() need override
//////////////////////////////////////////////////////////////////////////////
class ReCreation :
	public Game
{
private:
	//game items	
	TextureManager playerTexture;	//texture of player
	Image player;						//image of player

public:
	ReCreation(void);
	virtual ~ReCreation(void);

	//Initialize game
	void initialize(HWND hwnd);
	void update();
	void ai();
	void collisions();
	void render();
	void releaseAll();
	void resetAll();

};
