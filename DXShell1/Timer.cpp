#include "Timer.h"


/*
Function:Timer
Param:None
Return:None
Des:This is the constructor
*/
Timer::Timer()
{
	StartTime = 0;
	PauseTime = 0;
	paused = false;
	started = false;
}

/*
Function:Timer
Param:None
Return:None
Des:This is the constructor
*/
void Timer::Start()
{
	if (started) return;

	started = true;
	paused = false;
	StartTime = clock();
}


/*
Function:Reset
Param:None
Return:None
Des:This resets timer
*/
void Timer::Reset()
{

	paused = false;

	StartTime = clock();

}


/*
Function:GetDuration
Param:None
Return:unsigned long duration
Des:This gets duration since start
*/
unsigned long Timer::GetDuration()
{
	if (!started) return 0;

	if (paused) return PauseTime - StartTime;

	return clock() - StartTime;

}

/*
Function:IsStarted
Param:None
Return:bool status
Des:This tells if the timer is started 
*/
bool Timer::IsStarted()
{
	return started;
}

/*
Function:IsStopped
Param:None
Return:bool status
Des:This tells of the timer is stopped
*/
bool Timer::IsStopped()
{
	return !started;
}

/*
Function:IsPaused
Param:None
Return:None
Des:This tells if the timer is paused
*/
bool Timer::IsPaused()
{
	return paused;
}

/*
Function:IsActive
Param:None
Return:None
Des:This tells if the timer is active
*/
bool Timer::IsActive()
{
	return (!paused && started);
}

/*
Function:Pause
Param:None
Return:None
Des:This pauses timer
*/
void Timer::Pause()
{
	if (paused || !started) return;

	paused = true;
	PauseTime = clock();
}

/*
Function:Resume
Param:None
Return:None
Des:This resumes timer from pause
*/
void Timer::Resume()
{
	if (!paused) return;

	paused = false;
	StartTime += clock() - PauseTime;
}

/*
Function:Stop
Param:None
Return:None
Des:This stops timers
*/
void Timer::Stop()
{
	started = false;
}
