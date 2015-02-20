#ifndef MENUSTATE_H
#define MENUSTATE_
#include "SDL/SDL.h"
#include "gamestate.h"

class CMenuState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CMenuState* Instance() {
		return &m_MenuState;
	}

protected:
	CMenuState() { }

private:
	static CMenuState m_MenuState;

	SDL_Surface* bg;
};

#endif
