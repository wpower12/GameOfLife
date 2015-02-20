#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "SDL/SDL.h"
#include "gamestate.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include "gol.h"

class CPlayState : public CGameState
{
public:
	void Init();
	void Cleanup();
	
	void InitGoLSim( GoLSimulation* sim );
	
	void Pause();
	void Resume();

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	GoLSimulation* pGoL;	//A pointer to a game of life simulation object

	static CPlayState* Instance() {
		return &m_PlayState;
	}

protected:
	CPlayState() { }

private:
	static CPlayState m_PlayState;
	
	
};

#endif
