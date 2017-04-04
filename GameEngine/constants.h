#pragma once
//Author You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/11
//Last update 2017/2/16

#include <Windows.h>

#define SAFE_RELEASE(ptr) {if(ptr) {(ptr)->Release(); (ptr) = NULL;}}
#define SAFE_DELETE(ptr) {if(ptr) {delete (ptr); (ptr) = NULL;}}
#define TRANSCOLOR SETCOLOR_ARGB(0,255,0,255)	//transparant color (magenta)

//////////////////////////////////////////////////////////////////////////////
//Constant
//////////////////////////////////////////////////////////////////////////////

//Window
const char CLASS_NAME[] = "SpaceWar";
const char GAME_TITLE[] = "Game Engine Part 1";
const bool FULLSCREEN = false;	//Default is Window , or set fullscreen
static int GAME_WIDTH = 640;		//Game width(pixel)
static int GAME_HEIGHT = 480;		//Game height(pixel)

//Game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;									//The target frame rate
const float MIN_FRAME_RATE = 10.0f;								//The minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;			//Minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;	//Maximum time used in calculations

//graphic images
const char NEBULA_IMAGE[] = "textures\\player_01.png";
const char PLANET_IMAGE[] = "textures\\player_02.png";

//Key mapping
//This game use simple constant value to key mapping
//If use variable, save key mapping to data file, can reuse it
//««««««««««

const UCHAR CONSOLE_KEY = VK_OEM_3;		//@ key
const UCHAR ESC_KEY = VK_ESCAPE;			//Esc key
const UCHAR ALT_KEY = VK_MENU;				//Alt ley
const UCHAR ENTER_KEY = VK_RETURN;			//Enter key
const UCHAR SHIP1_LEFT_KEY = 'A';
const UCHAR SHIP1_RIGHT_KEY = 'D';
const UCHAR SHIP1_FORWARD_KEY = 'W';
const UCHAR SHIP1_FIRE_KEY = 'S';
const UCHAR SHIP2_LEFT_KEY = VK_LEFT;	//Left arrow key
const UCHAR SHIP2_RIGHT_KEY = VK_RIGHT;	//Right arrow key
const UCHAR SHIP2_FORWARD_KEY = VK_UP;	//Up arrow key
const UCHAR SHIP2_FIRE_KEY = VK_DOWN;		//Down arrow key

//ªªªªªªªªªª

