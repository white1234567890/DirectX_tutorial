//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/4/23
//LastUpdate 2017/4/23

#include "Audio.h"

//////////////////////////////////////////////////////////////////////////////
//constructor
//////////////////////////////////////////////////////////////////////////////
Audio::Audio(void)
{
}

//////////////////////////////////////////////////////////////////////////////
//destructor
//////////////////////////////////////////////////////////////////////////////
Audio::~Audio(void)
{
}

//////////////////////////////////////////////////////////////////////////////
//initialize audio engine
//////////////////////////////////////////////////////////////////////////////
HRESULT Audio::initialize()
{
	HRESULT result = E_FAIL;
	HANDLE hFile;
	DWORD fileSize;
	DWORD bytesRead;
	HANDLE hMapFile;

	//create XACT engine
	result = XACT3CreateEngine(0, &xactEngine);
	if(FAILED(result) || xactEngine == NULL)
		return E_FAIL;

	//initialize and create XACT runtime
	XACT_RUNTIME_PARAMETERS xactParam = {0};
	xactParam.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;	//if change another sound, XACT engine Prefetch how many
	result = xactEngine->Initialize(&xactParam);
	if(FAILED(result))
		return result;

	//create inmemory XACT wave bank file from wave bank created using XACT GUI tool
	//CreateFile parameter WAVE_BANK is defined in constants.h to already created wave bank
	//warn this parameter MUST be used RELATIVE PASS
	result = E_FAIL;	//default is failed
	hFile = CreateFile(WAVE_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(hFile, NULL);
		if(fileSize != -1)
		{
			hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, fileSize, NULL);
			if(hMapFile)
			{
				mapWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
				if(mapWaveBank)
					result = xactEngine->CreateInMemoryWaveBank(mapWaveBank,fileSize, 0, 0, &waveBank);
				//mapWaveBank keep handle to file
				//close unnecessary handle
				CloseHandle(hMapFile);
			}
		}
		//mapWaveBank keep handle to file
		//close unnecessary handle
		CloseHandle(hFile);
	}
	if(FAILED(result))
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	//read and register sound bank file that create using XACT
	result = E_FAIL;	//default is failed

	hFile = CreateFile(SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(hFile, NULL);
		if(fileSize != -1)
		{
			//allocate memory for sound bank
			soundBankData = new BYTE[fileSize];
			if(soundBankData)
			{
				if(0 != ReadFile(hFile, soundBankData, fileSize, &bytesRead, NULL))
					result = xactEngine->CreateSoundBank(soundBankData, fileSize, 0, 0, &soundBank);
			}
		}
		//soundBankData keep handle to file
		//close unnecessary handle
		CloseHandle(hFile);
	}
	if(FAILED(result))
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	return S_OK;
}
