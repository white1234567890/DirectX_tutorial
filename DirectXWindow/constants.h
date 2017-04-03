#pragma once
//Author You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/11
//Last update 2017/1/11

#include <Windows.h>

#define SAFE_RELEASE(ptr) {if(ptr) {(ptr)->Release(); (ptr) = NULL;}}
#define SAFE_DELETE(ptr) {if(ptr) {delete (ptr); (ptr) = NULL;}}


//Constant
//Window
const char CLASS_NAME[] = "SpaceWar";
const char APP_TITLE[] = "Game Engine Part 1";
const bool FULLSCREEN = false;	//Default is Window , or set fullscreen
static int GAME_WIDTH = 640;		//Game width(pixel)
static int GAME_HEIGHT = 480;		//Game height(pixel)

//Key mapping
const byte ESC_KEY = VK_ESCAPE;