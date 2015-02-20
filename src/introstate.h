
#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "SDL/SDL.h"
#include "gamestate.h"

class CIntroState : public CGameState	//Introstate inhereits GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CIntroState* Instance() {	//This is how we pass the single instance to other things, because the actual member is private
	
		return &m_IntroState;		//returns the pointer to the introstate instance
	}

protected:
	CIntroState() { }	//Protected constructor - singleton

private:
	static CIntroState m_IntroState;	//The instance of the singleton

	SDL_Surface* bg;
	SDL_Surface* fader;
	int alpha;
};

#endif
