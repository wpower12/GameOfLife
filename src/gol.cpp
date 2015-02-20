
#include "gol.h"

//Common SDL functions
SDL_Surface *load_image( std::string filename ){
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 255, 0, 255 ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL ){
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

GoLSimulation::GoLSimulation(){	//Constructor
	
	srand(time(NULL));
	for( int x = 0; x < X_SIZE; x++ ){
		for(int y = 0; y < Y_SIZE; y++ ){
			
			int randomValue = rand() % 100 ;	//random returns on a huge range, the modulo forces a ~1-100 range
			if( randomValue > 90 ){		//So ~10% of cells should start alive.
				simulationArray[x][y] = true;
			} else {
				simulationArray[x][y] = false;
			}
		}
	}
	
	pauseFlag = false;	//Start in an unpaused state
	randomFlag = false;
}

void GoLSimulation::Init_Sim()
{
	
	square = load_image( "block.bmp" );
	
	background = load_image("gol_background.bmp");


}

void GoLSimulation::setcell(int x, int y, bool state){	//Set the state of a single cell

	simulationArray[x][y] = state;
	
}

void GoLSimulation::draw_simulation( SDL_Surface* s ){	//Draws the current state of the simulation to the screen
	
	
	apply_surface( 0,0, background, s );
	//apply_surface( 32,32, square, s );	//This works!?  error is in update? randomization?
	
	for(int x = 0; x < X_SIZE; x++){	//Loop over x
		for(int y = 0; y < Y_SIZE; y++){	//Loop over	y
			if(simulationArray[x][y] == true){	//Check if alive
				apply_surface( SQUARE_SIZE*x, SQUARE_SIZE*y, square, s );	//If alive, draw a dot in that cell
			}
		}
	}

}

void GoLSimulation::create_glider(){	//For testing, sets a glider pattern in the middle of the screen

	simulationArray[11][4] = true;
	simulationArray[12][5] = true;
	simulationArray[10][1] = true;
	simulationArray[11][6] = true;
	simulationArray[18][4] = true;
	
	simulationArray[20][6] = true;
	simulationArray[1][6] = true;
	simulationArray[21][6] = true;
	
}

void GoLSimulation::update_simulation(){	//Updates the state of the simulation

	bool temp_array[X_SIZE][Y_SIZE];	//Temporary array for calculations
	
	for(int x = 0; x < X_SIZE; x++){	//Store the current state in the temp array
		for(int y = 0; y < Y_SIZE; y++){
			temp_array[x][y] = simulationArray[x][y];
		}
	}
	
	for(int x = 1; x < (X_SIZE - 1); x++){	//Interior update loop  
		for(int y =1; y < (Y_SIZE - 1); y++) {
			
			int aliveCount = 0;	//Will track # of alive neighbors
			//Neighbor Checks
				if( temp_array[x-1][y-1] == true ){ aliveCount++; }
				if( temp_array[x-1][y] == true ){ aliveCount++; }
				if( temp_array[x-1][y+1] == true ){ aliveCount++; }
				if( temp_array[x][y-1] == true ){ aliveCount++; }
				if( temp_array[x][y+1] == true ){ aliveCount++; }
				if( temp_array[x+1][y-1] == true ){ aliveCount++; }
				if( temp_array[x+1][y] == true ){ aliveCount++; }
				if( temp_array[x+1][y+1] == true ){ aliveCount++; }
			//Rule Checks
				//Loneliness - Alive + (1 or 0 Live Neighbors) = Death
				if( (temp_array[x][y] == true) && (aliveCount <= 1 ) ){
					simulationArray[x][y] = false;
				}
				//Survive - Alive + (2 or 3 Live Neightbors) = Stay Alive
				if( (temp_array[x][y] == true) && (aliveCount == 2 || aliveCount == 3) ) {
					simulationArray[x][y] = true;
				}
				//Overcrowding - Alive + (4 or more Live Neighbors) = Death
				if( (temp_array[x][y] == true) && (aliveCount > 3 ) ){
					simulationArray[x][y] = false;
				}				
				//Birth - Dead + (3 Live Neighbors) = Alive
				if( (temp_array[x][y] == false) && (aliveCount == 3 ) ){
					simulationArray[x][y] = true;
				}

		}
	}
	
	//Edge Cases
	for(int x = 1; x < (X_SIZE - 1); x++){	//Top Row edge case	- y = 0
	
			int aliveCount = 0;	//Will track # of alive neighbors
			//Neighbor Checks
				if( temp_array[x-1][Y_SIZE - 1] == true ){ aliveCount++; }
				if( temp_array[x-1][0] == true ){ aliveCount++; }
				if( temp_array[x-1][1] == true ){ aliveCount++; }
				if( temp_array[x][Y_SIZE - 1] == true ){ aliveCount++; }
				if( temp_array[x][1] == true ){ aliveCount++; }
				if( temp_array[x+1][Y_SIZE - 1] == true ){ aliveCount++; }
				if( temp_array[x+1][0] == true ){ aliveCount++; }
				if( temp_array[x+1][1] == true ){ aliveCount++; }
			//Rule Checks
				//Loneliness - Alive + (1 or 0 Live Neighbors) = Death
				if( (temp_array[x][0] == true) && (aliveCount <= 1 ) ){
					simulationArray[x][0] = false;
				}
				//Survive - Alive + (2 or 3 Live Neightbors) = Stay Alive
				if( (temp_array[x][0] == true) && (aliveCount == 2 || aliveCount == 3) ) {
					simulationArray[x][0] = true;
				}
				//Overcrowding - Alive + (4 or more Live Neighbors) = Death
				if( (temp_array[x][0] == true) && (aliveCount > 3 ) ){
					simulationArray[x][0] = false;
				}				
				//Birth - Dead + (3 Live Neighbors) = Alive
				if( (temp_array[x][0] == false) && (aliveCount == 3 ) ){
					simulationArray[x][0] = true;
				}	
	}
	
	for(int x = 1; x < (X_SIZE - 1); x++){	//Bottom Row edge case y = 23
	
			int aliveCount = 0;	//Will track # of alive neighbors
			//Neighbor Checks
				if( temp_array[x-1][Y_SIZE - 2] == true ){ aliveCount++; }
				if( temp_array[x-1][Y_SIZE - 1] == true ){ aliveCount++; }
				if( temp_array[x-1][0] == true ){ aliveCount++; }
				if( temp_array[x][Y_SIZE - 2] == true ){ aliveCount++; }
				if( temp_array[x][0] == true ){ aliveCount++; }
				if( temp_array[x+1][Y_SIZE - 2] == true ){ aliveCount++; }
				if( temp_array[x+1][Y_SIZE - 1] == true ){ aliveCount++; }
				if( temp_array[x+1][0] == true ){ aliveCount++; }
			//Rule Checks
				//Loneliness - Alive + (1 or 0 Live Neighbors) = Death
				if( (temp_array[x][Y_SIZE - 1] == true) && (aliveCount <= 1 ) ){
					simulationArray[x][Y_SIZE - 1] = false;
				}
				//Survive - Alive + (2 or 3 Live Neightbors) = Stay Alive
				if( (temp_array[x][0] == true) && (aliveCount == 2 || aliveCount == 3) ) {
					simulationArray[x][Y_SIZE - 1] = true;
				}
				//Overcrowding - Alive + (4 or more Live Neighbors) = Death
				if( (temp_array[x][0] == true) && (aliveCount > 3 ) ){
					simulationArray[x][Y_SIZE - 1] = false;
				}				
				//Birth - Dead + (3 Live Neighbors) = Alive
				if( (temp_array[x][0] == false) && (aliveCount == 3 ) ){
					simulationArray[x][Y_SIZE - 1] = true;
				}	
	
	}
	
	for(int y = 1; y < (Y_SIZE - 1); y++){	//Left Side edge case	x = 0
		int aliveCount = 0;	//Will track # of alive neighbors
			//Neighbor Checks
				if( temp_array[X_SIZE - 1][y-1] == true ){ aliveCount++; }
				if( temp_array[X_SIZE - 1][y] == true ){ aliveCount++; }
				if( temp_array[X_SIZE - 1][y+1] == true ){ aliveCount++; }
				if( temp_array[0][y-1] == true ){ aliveCount++; }
				if( temp_array[0][y+1] == true ){ aliveCount++; }
				if( temp_array[1][y-1] == true ){ aliveCount++; }
				if( temp_array[1][y] == true ){ aliveCount++; }
				if( temp_array[1][y+1] == true ){ aliveCount++; }
			//Rule Checks
				//Loneliness - Alive + (1 or 0 Live Neighbors) = Death
				if( (temp_array[0][y] == true) && (aliveCount <= 1 ) ){
					simulationArray[0][y] = false;
				}
				//Survive - Alive + (2 or 3 Live Neightbors) = Stay Alive
				if( (temp_array[0][y] == true) && (aliveCount == 2 || aliveCount == 3) ) {
					simulationArray[0][y] = true;
				}
				//Overcrowding - Alive + (4 or more Live Neighbors) = Death
				if( (temp_array[0][y] == true) && (aliveCount > 3 ) ){
					simulationArray[0][y] = false;
				}				
				//Birth - Dead + (3 Live Neighbors) = Alive
				if( (temp_array[0][y] == false) && (aliveCount == 3 ) ){
					simulationArray[0][y] = true;
				}
	}
	
	for(int y = 1; y < Y_SIZE - 1; y++){	//Right Side edge case - x = 23
		int aliveCount = 0;	//Will track # of alive neighbors
			//Neighbor Checks
				if( temp_array[X_SIZE - 2][y-1] == true ){ aliveCount++; }
				if( temp_array[X_SIZE - 2][y] == true ){ aliveCount++; }
				if( temp_array[X_SIZE - 2][y+1] == true ){ aliveCount++; }
				if( temp_array[X_SIZE - 1][y-1] == true ){ aliveCount++; }
				if( temp_array[X_SIZE - 1][y+1] == true ){ aliveCount++; }
				if( temp_array[0][y-1] == true ){ aliveCount++; }
				if( temp_array[0][y] == true ){ aliveCount++; }
				if( temp_array[0][y+1] == true ){ aliveCount++; }
			//Rule Checks
				//Loneliness - Alive + (1 or 0 Live Neighbors) = Death
				if( (temp_array[X_SIZE - 1][y] == true) && (aliveCount <= 1 ) ){
					simulationArray[X_SIZE - 1][y] = false;
				}
				//Survive - Alive + (2 or 3 Live Neightbors) = Stay Alive
				if( (temp_array[X_SIZE - 1][y] == true) && (aliveCount == 2 || aliveCount == 3) ) {
					simulationArray[X_SIZE - 1][y] = true;
				}
				//Overcrowding - Alive + (4 or more Live Neighbors) = Death
				if( (temp_array[X_SIZE - 1][y] == true) && (aliveCount > 3 ) ){
					simulationArray[X_SIZE - 1][y] = false;
				}				
				//Birth - Dead + (3 Live Neighbors) = Alive
				if( (temp_array[X_SIZE - 1][y] == false) && (aliveCount == 3 ) ){
					simulationArray[X_SIZE - 1][y] = true;
				}
	}
	//Corner Cases
	int aliveCount = 0;	
		//Top Left x = 0, y = 0
		if( temp_array[X_SIZE - 1][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 1][0] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 1][1] == true ){ aliveCount++; }
		if( temp_array[0][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[0][1] == true ){ aliveCount++; }
		if( temp_array[1][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[1][0] == true ){ aliveCount++; }
		if( temp_array[1][1] == true ){ aliveCount++; }
		//Rule Checks
		//Loneliness - Alive + (1 or 0 Live Neighbors) = Death
		if( (temp_array[0][0] == true) && (aliveCount <= 1 ) ){
			simulationArray[0][0] = false;
		}
		//Survive - Alive + (2 or 3 Live Neightbors) = Stay Alive
		if( (temp_array[0][0] == true) && (aliveCount == 2 || aliveCount == 3) ) {
			simulationArray[0][0] = true;
		}
		//Overcrowding - Alive + (4 or more Live Neighbors) = Death
		if( (temp_array[0][0] == true) && (aliveCount > 3 ) ){
			simulationArray[0][0] = false;
		}				
		//Birth - Dead + (3 Live Neighbors) = Alive
		if( (temp_array[0][0] == false) && (aliveCount == 3 ) ){
			simulationArray[0][0] = true;
		}
		aliveCount = 0;
		//Top Right x = X_SIZE - 1, y = 0
		if( temp_array[X_SIZE - 2][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 2][0] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 2][1] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 2][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 2][1] == true ){ aliveCount++; }
		if( temp_array[0][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[0][0] == true ){ aliveCount++; }
		if( temp_array[0][1] == true ){ aliveCount++; }
		//Rule Checks
		//Loneliness - Alive + (1 or 0 Live Neighbors) = Death
		if( (temp_array[X_SIZE - 1][0] == true) && (aliveCount <= 1 ) ){
			simulationArray[X_SIZE - 1][0] = false;
		}
		//Survive - Alive + (2 or 3 Live Neightbors) = Stay Alive
		if( (temp_array[X_SIZE - 1][0] == true) && (aliveCount == 2 || aliveCount == 3) ) {
			simulationArray[X_SIZE - 1][0] = true;
		}
		//Overcrowding - Alive + (4 or more Live Neighbors) = Death
		if( (temp_array[X_SIZE - 1][0] == true) && (aliveCount > 3 ) ){
			simulationArray[X_SIZE - 1][0] = false;
		}				
		//Birth - Dead + (3 Live Neighbors) = Alive
		if( (temp_array[X_SIZE - 1][0] == false) && (aliveCount == 3 ) ){
			simulationArray[X_SIZE - 1][0] = true;
		}
		aliveCount = 0;		
		//Bottom Left x = 0, y = Y_SIZE - 1
		if( temp_array[X_SIZE - 1][Y_SIZE - 2] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 1][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 1][0] == true ){ aliveCount++; }
		if( temp_array[0][Y_SIZE - 2] == true ){ aliveCount++; }
		if( temp_array[0][0] == true ){ aliveCount++; }
		if( temp_array[1][Y_SIZE - 2] == true ){ aliveCount++; }
		if( temp_array[1][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[1][0] == true ){ aliveCount++; }
		//Rule Checks
		//Loneliness - Alive + (1 or 0 Live Neighbors) = Death
		if( (temp_array[0][Y_SIZE - 1] == true) && (aliveCount <= 1 ) ){
			simulationArray[0][Y_SIZE - 1] = false;
		}
		//Survive - Alive + (2 or 3 Live Neightbors) = Stay Alive
		if( (temp_array[0][Y_SIZE - 1] == true) && (aliveCount == 2 || aliveCount == 3) ) {
			simulationArray[0][Y_SIZE - 1] = true;
		}
		//Overcrowding - Alive + (4 or more Live Neighbors) = Death
		if( (temp_array[0][Y_SIZE - 1] == true) && (aliveCount > 3 ) ){
			simulationArray[0][Y_SIZE - 1] = false;
		}				
		//Birth - Dead + (3 Live Neighbors) = Alive
		if( (temp_array[0][Y_SIZE - 1] == false) && (aliveCount == 3 ) ){
			simulationArray[0][Y_SIZE - 1] = true;
		}
		aliveCount = 0;	
		//Bottom Right x = X_SIZE - 1 y = Y_SIZE - 1
		if( temp_array[X_SIZE - 2][Y_SIZE - 2] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 2][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 2][0] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 1][Y_SIZE - 2] == true ){ aliveCount++; }
		if( temp_array[X_SIZE - 1][0] == true ){ aliveCount++; }
		if( temp_array[0][Y_SIZE - 2] == true ){ aliveCount++; }
		if( temp_array[0][Y_SIZE - 1] == true ){ aliveCount++; }
		if( temp_array[0][0] == true ){ aliveCount++; }
		//Rule Checks
		//Loneliness - Alive + (1 or 0 Live Neighbors) = Death
		if( (temp_array[X_SIZE - 1][Y_SIZE - 1] == true) && (aliveCount <= 1 ) ){
			simulationArray[X_SIZE - 1][Y_SIZE - 1] = false;
		}
		//Survive - Alive + (2 or 3 Live Neightbors) = Stay Alive
		if( (temp_array[X_SIZE - 1][Y_SIZE - 1] == true) && (aliveCount == 2 || aliveCount == 3) ) {
			simulationArray[X_SIZE - 1][Y_SIZE - 1] = true;
		}
		//Overcrowding - Alive + (4 or more Live Neighbors) = Death
		if( (temp_array[X_SIZE - 1][Y_SIZE - 1] == true) && (aliveCount > 3 ) ){
			simulationArray[X_SIZE - 1][Y_SIZE - 1] = false;
		}				
		//Birth - Dead + (3 Live Neighbors) = Alive
		if( (temp_array[X_SIZE - 1][Y_SIZE - 1] == false) && (aliveCount == 3 ) ){
			simulationArray[X_SIZE - 1][Y_SIZE - 1] = true;
		}
		aliveCount = 0;	

		SDL_Delay( 200 );
}
	
void GoLSimulation::setRandom(){
	
	randomFlag = true;

}	
	
void GoLSimulation::resetRandom(){
	randomFlag = false;
}	
	
void GoLSimulation::randomize_simulation(){	//Sets the simulationArray to a random distribution
	
	srand(time(NULL));
	for( int x = 0; x < 32; x++ ){
		for(int y = 0; y < 23; y++ ){
			
			int randomValue = rand() % 100 ;	//random returns on a huge range, the modulo forces a ~1-100 range
			if( randomValue > 90 ){		//So ~10% of cells should start alive.
				simulationArray[x][y] = true;
			} else {
				simulationArray[x][y] = false;
			}
		}
	}
	
	randomFlag = false;
	
}
	
bool GoLSimulation::isRandom(){
	
	return randomFlag;
	
}
