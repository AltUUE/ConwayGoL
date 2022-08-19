#include <SDL2/SDL.h>
#include "../inc/Timer.h"

Timer::Timer(){
	timeCount = 0;
	timeMod = 0;
	_timerDelay = 32;
}

Timer::~Timer(){

}

void Timer::initTimer(){
	timerID = SDL_AddTimer(_timerDelay, callBackFunc, this);
}

Uint32 Timer::callBackFunc(Uint32 interval, void* param){
	Timer* temp = (Timer*)param;
	temp->timeCount = (temp->timeCount+1) % (1<<(5-temp->timeMod));
	// printf("2-> %d\n", (*(int*)param));
	return interval;
}
