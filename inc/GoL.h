#pragma once
#include <SDL2/SDL.h>
#include "Generation.h"
#include "Timer.h"

enum class GameState {INPUT, AUTOMATIC, STEPWISE, EXIT};

class GoL{
public:
	GoL();
	~GoL();

	void run();

private:
	void initSystems();
	void gameLoop();
	void processInput();
	void drawStripes();
	void drawGeneration();
	void handleInputMode(int, int);
	void readFromFile();
	void printHelp();
	void changeGameMode(int);
	void saveGameState();
	void randomize();

	SDL_Window* _window;
	SDL_Surface* _screenSurface;
	int _screenWidth;
	int _screenHeight;
	int _posx;
	int _posy;
	int _zoomState;
	int _sizeOfSquare;
	GameState _gameState;
	Generation _generation;
	Timer _timer;
	int _multi;
	bool _risingEdge;
};
