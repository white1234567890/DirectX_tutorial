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
const char CLASS_NAME[] = "ReCreation";
const char GAME_TITLE[] = "ReCreation";
const bool FULLSCREEN = false;	//Default is Window , or set fullscreen
static int GAME_WIDTH = 640;		//Game width(pixel)
static int GAME_HEIGHT = 480;		//Game height(pixel)

//Game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;									//The target frame rate
const float MIN_FRAME_RATE = 10.0f;								//The minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;			//Minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;	//Maximum time used in calculations
const float GRAVITY = 2000.0f;										//gravity accelaration (pixel per 1 second)

//graphics images
const char PLAYER_IMAGE[] = "textures\\player.png";	//picture of player
const int PLAYER_START_FRAME = 0;				//starting frame of player animation
const int PLAYER_END_FRAME = 3;					//last frame of player animation
const float PLAYER_ANIMATION_DELAY = 0.1f;	//time between frames of ship animation
const int PLAYER_COL = 2;							//player texture has 2 columns
const int PLAYER_WIDTH = 64;						//width of player image
const int PLAYER_HEIGHT = 64;						//height of player image
const float ROTATION_RATE = 180;					//How much rotate per second
const float SCALE_RATE = 0.2f;					//scale change per second
const float PLAYER_SPEED = 100.0f;					//how many pixels per second
const float PLAYER_SCALE = 1.5f;						//starting ship scale

//Key mapping
//This game use simple constant value to key mapping
//If use variable, save key mapping to data file, can reuse it
//Å´Å´Å´Å´Å´Å´Å´Å´Å´Å´

const UCHAR ESC_KEY = VK_ESCAPE;						//Esc key
const UCHAR ALT_KEY = VK_MENU;							//Alt ley
const UCHAR ENTER_KEY = VK_RETURN;						//Enter key
const UCHAR PLAYER_LEFT_KEY = VK_LEFT;				//Left arrow key
const UCHAR PLAYER_RIGHT_KEY = VK_RIGHT;			//Right arrow key
const UCHAR PLAYER_UP_KEY = VK_UP;						//Up arrow key
const UCHAR PLAYER_DOWN_KEY = VK_DOWN;				//Down arrow key
const UCHAR PLAYER_FIRE_KEY = 'Z';						//Z key
const UCHAR PLAYER_BOMB_KEY = 'X';						//X key
const UCHAR PLAYER_LOW_SPEED_KEY = VK_RSHIFT;	//right shift key
const UCHAR PLAYER_HIGH_SPEED_KEY = VK_SPACE;	//space key

//Å™Å™Å™Å™Å™Å™Å™Å™Å™Å™

//weapon types
enum WEAPON {TORPEDO, SHIP, PLANET};