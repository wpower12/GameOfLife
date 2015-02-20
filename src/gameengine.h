
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "SDL/SDL.h"

#include <vector>
using namespace std;

class CGameState;

class CGameEngine
{
public:

	void Init(const char* title, int width=640, int height=480, int bpp=0);
	void Cleanup();

	void ChangeState(CGameState* state);
	void PushState(CGameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return running; }
	void Quit() { running = false; }

	SDL_Surface* screen;

private:
	// the stack of states
	vector<CGameState*> states;

	bool running;
};

#endif
