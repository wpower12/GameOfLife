#include <stdio.h>
#include "SDL/SDL.h"
#include "gameengine.h"
#include "gamestate.h"

void CGameEngine::Init(const char* title, int width, int height, int bpp) {
    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // set the title bar text
    SDL_WM_SetCaption(title, title);

    // create the screen surface
    screen = SDL_SetVideoMode(width, height, bpp, 0);
    running = true;
    printf("CGameEngine Init\n");
}

void CGameEngine::Cleanup() {
    // cleanup the all states
    while (!states.empty()) {
        states.back()->Cleanup();
        states.pop_back();
    }
    printf("CGameEngine Cleanup\n");
    // shutdown SDL
    SDL_Quit();
}

//Changes to a new state - starts its init, and closes most recent state

void CGameEngine::ChangeState(CGameState* state) {
    // cleanup the current state
    if (!states.empty()) {
        states.back()->Cleanup(); //Cleanup last state in states
        states.pop_back(); //Delete last state	in states
    }
    // store and init the new state
    states.push_back(state); //add new state to the end of the state stack
    states.back()->Init(); //run the init() of the new state
}

//adds a state to the stack (pauses, but doesnt quit, the last state)    

void CGameEngine::PushState(CGameState * state) {
    // pause current state
    if (!states.empty()) {
        states.back()->Pause();
    }
    // store and init the new state
    states.push_back(state);
    states.back()->Init();
}

//Removes the last state, and unpauses the next state (the foil of PushState)
void CGameEngine::PopState() {
    // cleanup the current state
    if (!states.empty()) {
        states.back()->Cleanup();
        states.pop_back();
    }
    // resume previous state
    if (!states.empty()) {
        states.back()->Resume();
    }
}
//Tell recent state to handle its events 
void CGameEngine::HandleEvents() {
    states.back()->HandleEvents(this);
}

// Tell state to update 
void CGameEngine::Update() {
    states.back()->Update(this);
}
// Tell state to draw 
void CGameEngine::Draw() {
    // let the state draw the screen
    states.back()->Draw(this);
}
