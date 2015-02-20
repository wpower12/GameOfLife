
#include "gameengine.h"
#include "introstate.h"
#include "playstate.h"

#include "gol.h"

int main ( int argc, char *argv[] )
{
	CGameEngine game;	//Game Engine Object
	GoLSimulation gol;	//GoL Simulation Object
	
	// initialize the engine
	game.Init( "Game of Life", 1024, 672, 32 );
	
	CPlayState::Instance()->InitGoLSim( &gol );
	
	// load the intro
	game.ChangeState( CIntroState::Instance() );

	// main loop
	while ( game.Running() )
	{
		game.HandleEvents();
		game.Update();
		game.Draw();
	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}
