#pragma once

#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>


/*
Class: SoundClass
DEs: This class is from http://www.rastertek.com/dx10tut14.html
Used to implement the sound in the game
*/
class SoundClass
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize(HWND);
	void Shutdown();

	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char* file, int whichSound);
	void ShutdownWaveFile(int whichSound);

	bool PlayWaveFile(bool shoudLoop, int whichSound);


	enum SoundColletion { Background, Enemy, Weapon, Explosion, Accelerate,Temp };

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;


	//temp 
	IDirectSoundBuffer8* m_secondaryBuffer1;
	//my sound objects
	IDirectSoundBuffer8* background;
	IDirectSoundBuffer8* enemy;
	IDirectSoundBuffer8* weapon;
	IDirectSoundBuffer8* explosion;
	IDirectSoundBuffer8* accelerate;
};

#endif

