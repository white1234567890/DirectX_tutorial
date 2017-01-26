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
//This game use simple constant value to key mapping
//If use variable, save key mapping to data file, can reuse it

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
