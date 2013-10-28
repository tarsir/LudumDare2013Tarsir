#ifndef _SCROLL_H
#define _SCROLL_H
#include "general.h"
#include <cstring>

using std::string;

class Background {
	private:
		int xScrollRate, yScrollRate;
		int xCur, yCur;
		ImgRsrc* bgRsrc;
		string bgLabel;
	public:
		Background() {

		}
		Background(string lbl, int xInit = 0, int yInit = 0) {
			xScrollRate = yScrollRate = 0;
			bgLabel = lbl;
			bgRsrc = findTileByLabel(bgLabel);
			xCur = xInit, yCur = yInit;
		}

		void setXScrollRate(int newX) {
			xScrollRate = newX;
		}

		void update(bool xChange, bool yChange) {
			//ignore vertical direction for now
			int bgWidth = bgRsrc->getSfc()->w;
			SDL_Surface* bgSfc = bgRsrc->getSfc();
			if (xChange) {
				xCur -= xScrollRate;
			}
			if (xCur <= -bgWidth) {
				xCur = 0;
			}
			applySurface(bgSfc, screen, xCur, yCur);
			applySurface(bgSfc, screen, xCur + bgWidth-2, yCur);
		}



};

#endif
