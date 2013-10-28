#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "general.h"
#include "character.h"
#include <cstring>
using std::string;
using std::cin;
using std::cout;
using std::endl;
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <vector>

extern SDL_Surface* screen;
extern SDL_Rect dstrect;
extern const int SCRN_HEIGHT;
extern const int SCRN_WIDTH;

TTF_Font* font = NULL;
SDL_Color textColor = { 220, 131, 0 };
SDL_Surface* textSfc;

bool initGfx() {

	font = TTF_OpenFont("assets/exobold.ttf", 56);
	if (font == NULL) {
		printf("Couldn't load font!\n");
		return false;
	}
	return true;
}

 SDL_Surface* loadImage(const char* filename, bool alpha=true) {
	char* fullpath = new char[200];
	strcpy(fullpath, "assets/");
	strcat(fullpath, filename);
    cout << fullpath << endl;
    printf("%s\n", fullpath);


    SDL_Surface* bmp = IMG_Load(fullpath);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return false;
    }
    return bmp;
}


class ImgRsrc {
	private:
		SDL_Surface* image;
		string label;
	public:
		ImgRsrc(string lbl) {
			label = lbl;
			string fpath = lbl + ".png";
			image = loadImage(fpath.c_str());
		}
		string getLabel() {
			//string _label;
			//_label = label;
			return label;
		}
		SDL_Surface* getSfc() {
			SDL_Surface* ptrCpy = image;
			return ptrCpy;
		}

};

typedef std::vector<ImgRsrc*> imageRList;

imageRList sprites;
imageRList tiles;

bool addToSprites(ImgRsrc* rsrc) {
	sprites.push_back(rsrc);
}

bool addToTiles(ImgRsrc* rsrc) {
	tiles.push_back(rsrc);
}

ImgRsrc* findSpriteByLabel(string lbl) {
	for (imageRList::iterator it = sprites.begin(); it < sprites.end(); it++) {
		if ((*it)->getLabel() == lbl) {
			return *it;
		}
	}
	return NULL;
}

ImgRsrc* findTileByLabel(string lbl) {
	for (imageRList::iterator it = tiles.begin(); it < tiles.end(); it++) {
		if ((*it)->getLabel() == lbl) {
			return *it;
		}
	}
	return NULL;
}

bool addFilesToLists() {
	ImgRsrc* x = new ImgRsrc("sunshinetile");
	addToTiles(x);
	//delete x;

	x = new ImgRsrc("water");
	addToTiles(x);
	//delete x;

	x = new ImgRsrc("grass");
	addToTiles(x);

	x = new ImgRsrc("brick");
	addToTiles(x);

	x = new ImgRsrc("background");
	addToTiles(x);

	x = new ImgRsrc("greything");
	addToTiles(x);

	x = new ImgRsrc("dirttrack");
	addToTiles(x);

	x = new ImgRsrc("goal");
	addToTiles(x);

	x = new ImgRsrc("splash");
	addToTiles(x);

	x = new ImgRsrc("score");
	addToTiles(x);

	x = new ImgRsrc("cursor");
	addToTiles(x);

	x = new ImgRsrc("runnyguymain");
	addToTiles(x);

	x = new ImgRsrc("star");
	addToTiles(x);

	x = new ImgRsrc("stage1");
	addToTiles(x);

	x = new ImgRsrc("stage3");
	addToTiles(x);

	x = new ImgRsrc("dirttrack2");
	addToTiles(x);

	x = new ImgRsrc("stage3");
	addToTiles(x);

	x = new ImgRsrc("stage1");
	addToTiles(x);

	x = new ImgRsrc("track1");
	addToTiles(x);

	x = new ImgRsrc("stg4");
	addToTiles(x);

	x = new ImgRsrc("endscreen");
	addToTiles(x);
	x = new ImgRsrc("credits");
	addToTiles(x);

	x = new ImgRsrc("prelvl1");
	addToTiles(x);
	x = new ImgRsrc("prelvl2");
	addToTiles(x);
	x = new ImgRsrc("prelvl3");
	addToTiles(x);
	x = new ImgRsrc("prelvl4");
	addToTiles(x);

	x = new ImgRsrc("howplay");
	addToTiles(x);


	x = new ImgRsrc("bigduder");
	addToSprites(x);

	x = new ImgRsrc("test");
	addToSprites(x);
	//delete x;
	return true;
}


bool refresh(SDL_Surface* scrn = screen) {
	if (SDL_Flip(scrn) == -1) {
		printf("Shit hit the flip, bro: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool applySurface(SDL_Surface* src, SDL_Surface* dest, int x, int y, SDL_Rect* clip = NULL) {
	dstrect.x = x;
	dstrect.y = y;
	if (SDL_BlitSurface(src, clip, dest, &dstrect) == -1) {
		printf("Shit hit the blit, bro: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool makeText(string message, int x, int y) {
	textSfc = TTF_RenderText_Solid(font, message.c_str(), textColor);
	if (textSfc == NULL) { return false; }
	applySurface(textSfc, screen, x, y);
	SDL_FreeSurface(textSfc);
}


//camera junk

class Camera {
	private:
		SDL_Rect camRect;
		Character* target;
	public:
		Camera() {
			 camRect.x = camRect.y = 0;
			 camRect.w = SCRN_WIDTH;
			 camRect.h = SCRN_HEIGHT;
			 target = NULL;
		}

		void setTarget(Character* tgt) {
			target = tgt;
		}

		void centerCameraOnTarget() {
			camRect.x = target->getXPos() - SCRN_WIDTH/2;
			camRect.y = target->getYPos() - SCRN_HEIGHT/2;
		}

		void keepInBounds(int xMax, int yMax) {
			if (camRect.x + camRect.w > xMax) {
				camRect.x = xMax - camRect.w;
			}
			if (camRect.y + camRect.h > yMax) {
				camRect.y = yMax - camRect.w;
			}
			if (camRect.x < 0) camRect.x = 0;
			if (camRect.y < 0) camRect.y = 0;
		}
		SDL_Rect getCam() { return camRect; }
 };

#endif


//WHAT THE FUUUUUUUUUUUUUUUUCK
