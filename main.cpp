//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>

#include "game.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

SDL_Rect test = {200, 200, 200u, 200u};
SDL_Rect* ptr = &test;

Uint32 color = 0x00FFFF00;

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else	{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

void close() {
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] ) {
	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    game theGame(gScreenSurface);
    theGame.spaceBar();

    Uint32 previousTime = SDL_GetTicks();

    //While application is running
    while( !quit ) {
      //Handle events on queue
      while( SDL_PollEvent( &e ) != 0 ) {
        //User requests quit
        if( e.type == SDL_QUIT ) {
          quit = true;
        }
        //User presses a key
        else if( e.type == SDL_KEYDOWN ) {
          //Select surfaces based on key press
          switch( e.key.keysym.sym ) {
            case SDLK_SPACE:
              theGame.spaceBar();
              break;
            default:
              break;
          }
        }
      }

      const Uint8 *state = SDL_GetKeyboardState(NULL);

      if (state[SDL_SCANCODE_LEFT]) {
        theGame.movePaddle(-PADDLE_SPEED*SPEED_FACTOR);
      }
      else if (state[SDL_SCANCODE_RIGHT]) {
        theGame.movePaddle(PADDLE_SPEED*SPEED_FACTOR);
      }

      Uint32 currentTime = SDL_GetTicks();
      Uint32 timeElapsed = currentTime - previousTime;
      previousTime = currentTime;

      theGame.update(timeElapsed);

      // clear screen and draw
      SDL_FillRect(gScreenSurface, NULL, 0x00000000);
      theGame.draw();

      //Update the surface
      SDL_UpdateWindowSurface(gWindow);

      // frame rate cap: about 60 fps
      if (timeElapsed < 8) {
        Uint32 wait = 8 - timeElapsed;
        SDL_Delay(wait);
      }
    }

	}

	//Free resources and close SDL
	close();

	return 0;
}
