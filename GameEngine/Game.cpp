#include "Game.h"


Game::Game(void)
{
	//Scures memory and initialize Input
	input = new Input();

	//Any other initialization will call input->initialize() after

	paused = false;
	graphics = NULL;
	initialized = false;
}


Game::~Game(void)
{
	//Release all memories
	deleteAll();

	ShowCursor(true);
}