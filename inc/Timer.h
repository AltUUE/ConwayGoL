#pragma once

class Timer{
public:
	Timer();
	~Timer();

	void initTimer();

	int timeCount;
	int timeMod;
	SDL_TimerID timerID;

private:
	static Uint32 callBackFunc(Uint32, void*);

	int _timerDelay;
};