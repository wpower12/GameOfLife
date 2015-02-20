#include <stdio.h>
#include "SDL/SDL.h"
#include "gamestate.h"
#include "gameengine.h"
#include "playstate.h"
#include "menustate.h"

CPlayState CPlayState::m_PlayState;

void CPlayState::InitGoLSim( GoLSimulation* sim ) 
{
	pGoL = sim;
	
}

void CPlayState::Init()
{
	pGoL->Init_Sim();
}

void CPlayState::Cleanup()
{

	
	printf("CPlayState Cleanup\n");
}

void CPlayState::Pause()
{
	printf("CPlayState Pause\n");
}

void CPlayState::Resume()
{
	printf("CPlayState Resume\n");
}

void CPlayState::HandleEvents(CGameEngine* game)
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game->Quit();
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						game->Quit();
						break;
					case SDLK_m:
						game->PushState( CMenuState::Instance() );
						break;
					case SDLK_r:
						pGoL->setRandom();
				}
				break;
		}
	}
}

void CPlayState::Update(CGameEngine* game)
{
	if( pGoL->isRandom() ){
		pGoL->randomize_simulation();
	}
	
	pGoL->update_simulation();
}

void CPlayState::Draw(CGameEngine* game)
{
	
	pGoL->draw_simulation( game->screen );
	
	SDL_UpdateRect(game->screen, 0, 0, 0, 0);
}

