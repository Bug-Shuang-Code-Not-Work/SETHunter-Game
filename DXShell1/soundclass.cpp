#include "soundclass.h"


/*
Function: SoundClass
Param: None
Return: None
Des: Constructor
*/
SoundClass::SoundClass()
{
	m_DirectSound = 0;
	m_primaryBuffer = 0;
	m_secondaryBuffer1 = 0;


	background = 0;
	enemy = 0;
	weapon = 0; 
	explosion = 0;
	accelerate = 0;


}


SoundClass::SoundClass(const SoundClass& other)
{
}


SoundClass::~SoundClass()
{
}



/*
Function: Initialize
Param: None
Return: None
Des: Initialize the sound in the window
*/
bool SoundClass::Initialize(HWND hwnd)
{
	bool result;


	// Initialize direct sound and the primary sound buffer.
	result = InitializeDirectSound(hwnd);
	if (!result)
	{
		return false;
	}

	// Load a wave audio file onto a secondary buffer.
	//result = LoadWaveFile("../Engine/data/sound01.wav", &m_secondaryBuffer1);
//	if(!result)
//	{
//		return false;
//	}

	// Play the wave file now that it has been loaded.
//	result = PlayWaveFile();
//	if(!result)
//	{
//		return false;
//	}

//	return true;


	LoadWaveFile("background-converted.wav", Background);
	LoadWaveFile("accelerate-converted.wav", Accelerate);
	LoadWaveFile("enemy-converted.wav", Enemy);
	LoadWaveFile("explosion-converted.wav", Explosion);
	LoadWaveFile("weapon-converted.wav", Weapon);

}

/*
Function: Shutdown
Param: None
Return: None
Des: Shutdown all handles
*/
void SoundClass::Shutdown()
{
	// Release the secondary buffer.
	for (int i = 0; i < 5; i++)
	{
		ShutdownWaveFile(i);
	}
	

	// Shutdown the Direct Sound API.
	ShutdownDirectSound();

	return;
}

/*
Function: InitializeDirectSound
Param: HWND hwnd
Return: None
Des: Specific initialization
*/
bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;


	// Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

/*
Function: ShutdownDirectSound
Param: None
Return: None
Des: Constructor
*/
void SoundClass::ShutdownDirectSound()
{
	// Release the primary sound buffer pointer.
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// Release the direct sound interface pointer.
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}

	return;
}

/*
Function: SoundClass
Param: char* filename, int whichSound (File Path, soundcollection reference)
Return: true if loaded false otherwise
Des: this method loads the file into the program
*/
bool SoundClass::LoadWaveFile(char* filename, int whichSound)
{



	IDirectSoundBuffer8** secondaryBuffer = &m_secondaryBuffer1;

	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;


	// Open the wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// Check that the wave file was recorded in stereo format.
	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.




	switch (whichSound)
	{
	case Background:
		secondaryBuffer = &background;
		break;
	case Enemy:
		secondaryBuffer = &enemy;
		break;
	case Explosion:
		secondaryBuffer = &explosion;
		break;
	case Accelerate:
		secondaryBuffer = &accelerate;
		break;
	case Weapon:
		secondaryBuffer = &weapon;
		break;
	case Temp:
		secondaryBuffer = &m_secondaryBuffer1;
		break;
	}


	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	return true;
}

/*
Function: ShutdownWaveFile
Param: int whichSound ( sound collection reference)
Return: None
Des: This method shut down the file which is playing
*/
void SoundClass::ShutdownWaveFile(int whichSound)
{
	// Release the secondary sound buffer.

	IDirectSoundBuffer8** temp = &m_secondaryBuffer1;

	switch (whichSound)
	{
	case Background:
		temp = &background;
		break;
	case Enemy:
		temp = &enemy;
			break;
	case Explosion:
		temp = &explosion;
		break;
	case Accelerate:
		temp = &accelerate;
		break;
	case Weapon:
		temp = &weapon;
		break;
	case Temp:
		temp = &m_secondaryBuffer1;
		break;
	}


	if (*temp)
	{
		(*temp)->Release();
		*temp = 0;
	}

	return;
}

/*
Function: PlayWaveFile
Param: bool shouldLoop, int whichSound ( should play in loop,    sound collection reference)
Return: true if succeed
Des: This file plays the sound file
*/
bool SoundClass::PlayWaveFile(bool shouldLoop, int whichSound)
{
	HRESULT result;

	switch (whichSound)
	{
	case Background:
		m_secondaryBuffer1 = background;
		break;
	case Enemy:
		m_secondaryBuffer1 = enemy;
		break;
	case Weapon:
		m_secondaryBuffer1 = weapon;
		break;
	case Explosion:
		m_secondaryBuffer1 = explosion;
		break;
	case Accelerate:
		m_secondaryBuffer1 = accelerate;
		break;

	}

	// Set position at the beginning of the sound buffer.
	result = m_secondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	// Set volume of the buffer to 100%.
	result = m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		return false;
	}

	// Play the contents of the secondary sound buffer.
	if (shouldLoop)
	{
		result = m_secondaryBuffer1->Play(0, 0, DSBPLAY_LOOPING);
	}
	
	else
	{
		result = m_secondaryBuffer1->Play(0, 0, 0);
	}
	
	if (FAILED(result))
	{
		return false;
	}

	return true;
}