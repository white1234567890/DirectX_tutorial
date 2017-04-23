//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/4/23
//LastUpdate 2017/4/23

#pragma once

#define Win32_LEAN_AND_MEAN

#include <xact3.h>
#include "constants.h"

//it is easy that you want to play sound cue using XACT audio engine
//need steps as follows
//1.	initialize XACT engine
//2.	create XACT wave bank more than 1
//3.	create XACT sound bank more than 1
//4.	play sound using sound cue
//5.	call DoWork() from XACT engine periodic
//6.	when finish, clean up them
class Audio
{
	//properties
private:
	IXACT3Engine* xactEngine;		//pointer to XACT sound engine
	IXACT3WaveBank* waveBank;		//pointer to XACT wave bank
	IXACT3SoundBank* soundBank;	//pointer to XACT sound bank
	XACTINDEX cueI;						//XACT sound index
	void* mapWaveBank;					//call UnmapViewOfFile() to release file
	void* soundBankData;
	bool coInitialized;				//set true if coInitialize is successful
	

public:
	//constructor
	Audio(void);
	//destructor
	virtual ~Audio(void);

	//member function

	//initialize audio
	HRESULT initialize();

	//perform periodic sound engine task
	void run();

	//play sound specified by cue from sound bank
	//if cue does not exist no error occurs, there is simply no sound played
	void playCue(const char cue[]);

	//stop a playing sound specified by cue from sound bank
	//if cue does not exist no error occurs
	void stopCue(const char cue[]);
};

