#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include "GoL.h"
#include "Generation.h"
#include "Color.h"
#include "Timer.h"

void fatalError(std::string errorString){
	std::cout << errorString << std::endl;
	std::cout << "enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
}

GoL::GoL(){
	_window = nullptr;
	_screenSurface = nullptr;
	_screenWidth = 1024;
	_screenHeight = 512;
	_gameState = GameState::INPUT;
	_posx = 0;
	_posy = 0;
	_zoomState = 3;
	_sizeOfSquare = (1 << _zoomState);
	_multi = 0;
	_risingEdge = 1;

	std::cout << "Welcome to Game of Life 2.0!" << std::endl;
	std::cout << "Press h to view controls!" << std::endl;
}

GoL::~GoL(){

}

void GoL::run(){
	initSystems();
	gameLoop();
}

void GoL::initSystems(){
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);

	_window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_SHOWN);
	if(_window == nullptr) fatalError("SDL Window could not be created!");

	SDL_RaiseWindow(_window);

	_screenSurface = SDL_GetWindowSurface(_window);
	if(_screenSurface == nullptr) fatalError("SDL Surface could not be created!");

	// if(_gameState == GameState::AUTOMATIC)
}

void GoL::gameLoop(){
	Uint64 start, end;
	float elapsed, delta;
	while(_gameState != GameState::EXIT){
		start = SDL_GetPerformanceCounter();	
		if(_generation.generation.empty()) _gameState = GameState::INPUT;
		
		processInput();
		drawGeneration();
		
		if(_timer.timeCount == 0 && _gameState == GameState::AUTOMATIC){
			_timer.timeCount = 1;
			_generation.calculateNextGen();
		}
		end = SDL_GetPerformanceCounter();
		elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
		delta = 16.6f - elapsed * 1000.0f;
		if(delta > 0) SDL_Delay(delta);
	}	
}

void GoL::processInput(){
	SDL_Event evnt;
	if(SDL_PollEvent(&evnt)){
		if(evnt.type == SDL_QUIT) changeGameMode(4);
		if(evnt.type == SDL_KEYDOWN){
			if(evnt.key.keysym.sym == SDLK_w) _posy += 1;
			if(evnt.key.keysym.sym == SDLK_a) _posx -= 1;
			if(evnt.key.keysym.sym == SDLK_s) _posy -= 1;
			if(evnt.key.keysym.sym == SDLK_d) _posx += 1;

			if(evnt.key.keysym.sym == SDLK_j && _timer.timeMod < 5 && _gameState == GameState::AUTOMATIC) _timer.timeMod += 1;
			if(evnt.key.keysym.sym == SDLK_k && _timer.timeMod > -1 && _gameState == GameState::AUTOMATIC) _timer.timeMod -= 1;

			if((evnt.key.keysym.sym == SDLK_KP_ENTER || evnt.key.keysym.sym == SDLK_RETURN) && _gameState != GameState::AUTOMATIC){
				if(_generation.generation.empty()) readFromFile();
				changeGameMode(2);
			}

			if(evnt.key.keysym.sym == SDLK_t){
				if(_gameState != GameState::STEPWISE){
					if(_generation.generation.empty()) readFromFile();
					changeGameMode(3);
				}
				else if(_gameState == GameState::STEPWISE){
					_generation.calculateNextGen();
				}			
			}

			if(evnt.key.keysym.sym == SDLK_i && _gameState != GameState::INPUT){
				changeGameMode(1);
			}

			if(evnt.key.keysym.sym == SDLK_h) printHelp();

			if(evnt.key.keysym.sym == SDLK_x){
				_generation.generation.clear();
				_multi = 0;
				_risingEdge = 1;
				changeGameMode(1);
			}

			if(evnt.key.keysym.sym == SDLK_0){
				_multi = 0;
				_risingEdge = 1;
			}

			if(evnt.key.keysym.sym == SDLK_0 || evnt.key.keysym.sym == SDLK_1 || evnt.key.keysym.sym == SDLK_2 || evnt.key.keysym.sym == SDLK_3 || evnt.key.keysym.sym == SDLK_4 || evnt.key.keysym.sym == SDLK_5 || evnt.key.keysym.sym == SDLK_6){
				if(evnt.key.keysym.sym == SDLK_1) _multi = 1;
				if(evnt.key.keysym.sym == SDLK_2) _multi = 2;
				if(evnt.key.keysym.sym == SDLK_3) _multi = 3;
				if(evnt.key.keysym.sym == SDLK_4) _multi = 4;
				if(evnt.key.keysym.sym == SDLK_5) _multi = 5;
				if(evnt.key.keysym.sym == SDLK_6) _multi = 6;

				if(_risingEdge){
					std::vector<std::set<std::pair<std::pair<int, int>, Color>, Generation::Cmp>::iterator> v;
					_risingEdge = 0;
					for(std::set<std::pair<std::pair<int, int>, Color>, Generation::Cmp>::iterator it=_generation.generation.begin(); it != _generation.generation.end(); it++){
						if(it->second == Color({0x00, 0x00, 0x00})) v.push_back(it);
					}
					for(std::set<std::pair<std::pair<int, int>, Color>, Generation::Cmp>::iterator iter : v){
						_generation.generation.erase(iter);
						_generation.generation.insert({{iter->first.first, iter->first.second}, Color((_multi & 4)/4 * 0xFF, (_multi & 2)/2 * 0xFF, (_multi & 1) * 0xFF)});
					}
				}
			}

			if(evnt.key.keysym.sym == SDLK_SPACE){
				saveGameState();
			}

			if(evnt.key.keysym.sym == SDLK_r){
				randomize();
			}
		}

		if(evnt.type == SDL_MOUSEBUTTONDOWN){
			if(evnt.button.button == SDL_BUTTON_LEFT && _gameState == GameState::INPUT){
				handleInputMode(evnt.button.x, _screenHeight - evnt.button.y);
			}
		}

		if(evnt.type == SDL_MOUSEWHEEL){
			if(evnt.wheel.y > 0){ // scroll up (zoom in)
				if(_zoomState < 7){
					_posx += 256/(1 << _zoomState);
					_posy += 128/(1 << _zoomState);
					_zoomState++;
				}
			}
			else if(evnt.wheel.y < 0){ // scroll down (zoom out)
				if(_zoomState > 0){
					_posx -= 512/(1 << _zoomState);
					_posy -= 256/(1 << _zoomState);
					_zoomState--;
				}
			}
		}
	}
}

void GoL::drawGeneration(){
	SDL_Rect rect;
	_sizeOfSquare = (1 << _zoomState);
	int xnum = _screenWidth/_sizeOfSquare;
	int ynum = _screenHeight/_sizeOfSquare;
	rect.h = _sizeOfSquare;
	rect.w = _sizeOfSquare;
	std::set<std::pair<std::pair<int,int>, Color>, Generation::Cmp>::iterator it;
	it = _generation.generation.lower_bound({{_posx, _posy}, Color(0x00, 0x00, 0x00)});
	SDL_FillRect(_screenSurface, nullptr, SDL_MapRGB(_screenSurface->format, 0xFF, 0xFF, 0xFF));
	for(; it != _generation.generation.upper_bound({{_posx+xnum-1, _posy+ynum-1}, Color(0x00, 0x00, 0x00)}); it++){
		rect.x = (it->first.first - _posx) * _sizeOfSquare;
		rect.y = ((_posy + ynum - 1) - it->first.second) * _sizeOfSquare;
		if(_multi == 0){
			SDL_FillRect(_screenSurface, &rect, SDL_MapRGB(_screenSurface->format, 0x00, 0x00, 0x00));
		}
		else{
			SDL_FillRect(_screenSurface, &rect, SDL_MapRGB(_screenSurface->format, it->second.rgb[0], it->second.rgb[1], it->second.rgb[2]));
		}
	}

	// vertical stripes
	rect.x = _sizeOfSquare * 3 / 4;
	rect.y = 0;
	rect.h = _screenHeight;
	rect.w = _sizeOfSquare / 4;
	for(int i = 0; i <= xnum - 1; i++){
		SDL_FillRect(_screenSurface, &rect, SDL_MapRGB(_screenSurface->format, 0x7F, 0x7F, 0x7F));
		rect.x += _sizeOfSquare;
	}

	// Horizontal Stripes
	rect.x = 0;
	rect.y = _sizeOfSquare * 3 / 4;
	rect.h = _sizeOfSquare / 4;
	rect.w = _screenWidth;
	for(int i = 0; i <= ynum - 1; i++){
		SDL_FillRect(_screenSurface, &rect, SDL_MapRGB(_screenSurface->format, 0x7F, 0x7F, 0x7F));
		rect.y += _sizeOfSquare;
	}

	SDL_UpdateWindowSurface(_window);
}

void GoL::handleInputMode(int x, int y){
	int xQuotient = x / _sizeOfSquare;
	int xRemainder = x % _sizeOfSquare;

	int yQuotient = y / _sizeOfSquare;
	int yRemainder = y % _sizeOfSquare;

	if(xRemainder <= _sizeOfSquare * 3 / 4 && yRemainder >= _sizeOfSquare / 4){
		std::set<std::pair<std::pair<int, int>, Color>, Generation::Cmp>::iterator it = _generation.generation.find({{_posx + xQuotient, _posy + yQuotient}, Color(0x00, 0x00, 0x00)});
		if(it != _generation.generation.end()){
			_generation.generation.erase(it);
		}
		else{
			_generation.generation.insert({{_posx + xQuotient, _posy + yQuotient}, Color((_multi & 4) * 0xFF, (_multi & 2) * 0xFF, (_multi & 1) * 0xFF)}); // 300 iq award
		}
	}
}

void GoL::readFromFile(){
	std::ifstream myFile("input.txt");
	std::string s;
	std::getline(myFile, s);
	int p1, p2, c1, c2, c3;
	Color clr(0x00, 0x00, 0x00);
	if(s == "rule: vanilla"){
		while(myFile >> p1){
			myFile >> p2;
			_generation.generation.insert({{p1, p2}, clr});
		}
	}
	if(s == "rule: multi"){
		while(myFile >> p1){
			myFile >> p2 >> c1 >> c2 >> c3;
			clr.rgb[0] = c1; clr.rgb[1] = c2; clr.rgb[2] = c3;
			_generation.generation.insert({{p1, p2}, clr});
			_multi = 1;
		}
	}
	myFile.close();
}

void GoL::printHelp(){
	std::cout << "Controls:\n";
	std::cout << "-> Use w, a, s, d to move around the map.\n";
	std::cout << "-> Press i for input mode (Game initially starts at input mode) and place living squares with left click (If square is already living it will be deleted).\n";
	std::cout << "-> Press enter for automatic mode (If you press enter with 0 living squares, game will read default input)\n";
	std::cout << "-> While in automatic mode, press j to speed up or press k to slow down.\n";
	std::cout << "-> Press t for stepwise mode and while in stepwise mode, press t to create new generation.\n";
	std::cout << "-> Press x to reset board.\n";
	std::cout << "-> Use mouse wheel to zoom in (scroll up) or zoom out (scroll down).\n";
	std::cout << "-> Use 1-6 to enter multilife mode where squares have colors. Press 0 to quit multilife.\n";
	std::cout << "-> When you enter multilife, all black squares will have colors assigned to them in accordance with your selection (1-6).\n";
	std::cout << "-> Note that when you enter and exit, colors are not deleted but rather hidden. If you enter again with another color, only the newly added squares will be affected.\n";
	std::cout << "-> Color of a newly birthed square is determined by the average of its neighbours.\n";
	std::cout << "-> Press space to save current generation and append to saves.txt.\n";
	std::cout << "-> If you press space in Multilife, the colors will be saved as well.\n";
	std::cout << "-> Press r to randomize the colors of squares.\n";
}

void GoL::changeGameMode(int mode){
	if(mode == 1){
		SDL_RemoveTimer(_timer.timerID);
		_gameState = GameState::INPUT;
	}

	if(mode == 2){
		_gameState = GameState::AUTOMATIC;
		_timer.timeMod = 3;
		_timer.initTimer();
	}

	if(mode == 3){
		SDL_RemoveTimer(_timer.timerID);
		_gameState = GameState::STEPWISE;
	}

	if(mode == 4) _gameState = GameState::EXIT;
}

void GoL::saveGameState(){
	if(_gameState == GameState::AUTOMATIC)
		changeGameMode(3);
	
	std::fstream appFile("saves.txt", std::fstream::app);
	time_t tnow;
	time(&tnow);
	
	std::string savename;
	std::cout << "Save name: ";
	std::getline(std::cin, savename);
	SDL_RaiseWindow(_window);
	
	size_t nspaces=0;
	while(savename.length() > nspaces && savename[nspaces] == ' ')
		nspaces++;
	appFile << "# " << (savename.length()-nspaces ? savename.substr(nspaces) : "Unnamed") << '\n';
	
	appFile << "# " << ctime(&tnow);
	std::set<std::pair<std::pair<int, int>, Color>, Generation::Cmp>::iterator it;
	if(_multi == 0){
		appFile << "rule: vanilla\n";
		for(it = _generation.generation.begin(); it != _generation.generation.end(); it++) appFile << it->first.first << " " << it->first.second << " ";
	}
	else{
		appFile << "rule: multi\n";
		for(it = _generation.generation.begin(); it != _generation.generation.end(); it++) appFile << it->first.first << " " << it->first.second << " " << it->second.rgb[0] << " " << it->second.rgb[1] << " " << it->second.rgb[2] << " ";
	}
	appFile << "\n\n"; appFile.close();
}

void GoL::randomize(){
	_multi = 1;
	srand(time(0));
	int c1, c2, c3;
	std::vector<std::set<std::pair<std::pair<int, int>, Color>, Generation::Cmp>::iterator> itvec;
	for(std::set<std::pair<std::pair<int, int>, Color>, Generation::Cmp>::iterator it = _generation.generation.begin(); it != _generation.generation.end(); it++) itvec.push_back(it);

	for(std::set<std::pair<std::pair<int, int>, Color>, Generation::Cmp>::iterator it: itvec){
		c1 = rand() % 254 + 1; c2 = rand() % 254 + 1; c3 = rand() % 254 + 1;

		_generation.generation.erase(it);

		// _generation.generation.insert({{it->first.first, it->first.second}, Color(c1, c2, c3)});
		_generation.generation.insert({{it->first.first, it->first.second}, Color(0xFF*c1/(c1+c2+c3), 0xFF*c2/(c1+c2+c3), 0xFF*c3/(c1+c2+c3))});
	}
}
