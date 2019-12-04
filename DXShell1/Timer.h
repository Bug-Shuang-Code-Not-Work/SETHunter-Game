#pragma once

#include <ctime>
using namespace std;

/*
class: Timer
Des: This class functions as the stop watch to time the game process 
*/
class Timer
{   
	
	clock_t StartTime;
	clock_t PauseTime;

	bool started;
	bool paused;

public:
	Timer();

    // get ticks
	unsigned long GetDuration();
    
	//timer status
	bool IsStarted();
	bool IsStopped();
	bool IsPaused();
	bool IsActive();

    //timer action
	void Pause();
	void Resume();
	void Stop();
	void Start();
	void Reset();
};

