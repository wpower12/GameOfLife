/* Game of Life Simulation Class
	

 */
 #ifndef GOL_H
 
 #define X_SIZE 32
 #define Y_SIZE 21
 #define SQUARE_SIZE 32
 
 #define GOL_H
 #include "SDL/SDL.h"
 #include "SDL/SDL_image.h"
 #include <stdlib.h>
 #include <time.h>
 #include <string>
 
 class GoLSimulation {

	private:
		bool simulationArray[X_SIZE][Y_SIZE];	//Array that holds the state of the cells
		bool pauseFlag;		//Flag for paused state
		
		bool randomFlag;
		
		SDL_Surface* square;
		SDL_Surface* background;
		
	public:
		GoLSimulation();			//Constructor
		void Init_Sim();			//set things up (like surfaces)
		
		void update_simulation();	//Updates the cells in the simulation array
		void draw_simulation( SDL_Surface* s );		//Draws dots to the locations of live cells
		
		void setRandom();
		void resetRandom();
		bool isRandom();
		
		void randomize_simulation();	//Creates random dist of dots to start
		
		void setcell(int, int, bool);	//set an individual cell
		void create_glider();			//Places a glider pattern in the middle of the screen

};
#endif
