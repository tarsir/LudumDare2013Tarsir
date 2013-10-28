#ifndef _GENERAL_H
#define _GENERAL_H

#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include "timer.h"

using std::cin;
//Global variables
SDL_Surface* screen = NULL;
SDL_Rect dstrect; //only used for blitting
SDL_Event event;

bool camClip = true;

const int SCRN_HEIGHT = 480;
const int SCRN_WIDTH = 640;


/*typedef std::vector<SDL_Surface*> imageList;
imageList sprites;
imageList tiles;*/


/// Initialize fucking everything.
bool init() {
    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    if (TTF_Init() == -1) {
		return false;
	}
    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);

    if ( !screen )
    {
        printf("Unable to set video resolution: %s\n", SDL_GetError());
        return 1;
    }

    int flags=IMG_INIT_JPG|IMG_INIT_PNG;
	int initted=IMG_Init(flags);
	if(initted&flags != flags) {
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
		// handle error
	}
	fpsTimer.start();
}

#endif
