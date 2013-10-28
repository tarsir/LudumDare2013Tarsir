#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <cmath>
#include <cstring>
using std::string;
#include "general.h"
#include "timer.h"

int xVelMax = 30;
int yVelMax = 50;

class ImgRsrc;
extern SDL_Event event;
extern const int SCRN_HEIGHT;
extern const int SCRN_WIDTH;
extern bool camClip;

class Character {
	protected:
		int xPos, yPos;
		double xVel, yVel;
	private:
		ImgRsrc* iRsrc;
	public:
		string charLabel;
		Character(string lbl, ImgRsrc* rsrc) {
			charLabel = lbl;
			iRsrc = rsrc;
			xPos = 0;
			yPos = 0;
		}
		void setPosition(int x, int y) {
			xPos = x;
			yPos = y;
		}

		//direction is which direction the character comes from
		//0 = east, 1 = north-east, 2 = north, etc.
		void handleCollision (int x, int y, int direction) {
			switch (direction) {
				case 0:
					xPos = x + 16;
					break;
				case 1:
					xPos = x + 16;
					yPos = y - 32;
					break;
				case 2:
					yPos = y - 32;
					break;
				case 3:
					xPos = x - 16;
					yPos = y - 32;
					break;
				case 4:
					xPos = x - 16;
					break;
				case 5:
					xPos = x - 16;
					yPos = y + 32;
					break;
				case 6:
					yPos = y + 32;
					break;
				case 7:
					xPos = x + 16;
					yPos = y+32;
					break;
				default:
					//do nothing
					int y;
			}
		}

		int getDirection() {
			if (xVel > 0) {
				if (yVel > 0) {
					return 3;
				} else if (yVel < 0) {
					return 5;
				} else { return 4; }
			}
			else if (xVel < 0) {
				if (yVel > 0) {
					return 1;
				} else if (yVel < 0) {
					return 7;
				} else { return 0; }
			}
			else {
				if (yVel > 0) {
					return 2;
				}
				else if (yVel < 0) {
					return 6;
				}
			}
			return -1;
		}

		int getXPos() {
			return xPos;
		}
		int getYPos() {
			return yPos;
		}
		int getXVelocity() {
			return xVel;
		}
		void setXVelocity(int newXV) {
			xVel = newXV;
		}
};

class PlayerCharacter : public Character {
	private:
		int lastPress;
		int diffTime;
		bool wasLastD, firstPress;
		int raceDistance;
		int framesPerAnimClip, currentFrame, previousFrame;
		int maxVelocity;
	public:
		PlayerCharacter(string lbl, ImgRsrc* rsrc) : Character(lbl, rsrc) {
			xVel = yVel = 0;
			setPosition(0, 0);
			lastPress = 0;
			diffTime = 0;
			wasLastD = false;
			firstPress = true;
			framesPerAnimClip = 10;
			currentFrame = 0, previousFrame = 0;
			maxVelocity = 10;
		}

		void resetDistance() {
			raceDistance = 0;
		}

		void update(bool raceDone, bool& didTravel) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_KEYDOWN) {
					switch (event.key.keysym.sym) {
						/*case SDLK_LEFT:
							xVel -= 4;
							if (fabs(xVel) > xVelMax) {
								xVel = xVelMax * (xVel/fabs(xVel));
							}
							break;
						case SDLK_RIGHT:
							xVel += 4;
							if (fabs(xVel) > xVelMax) {
								xVel = xVelMax * (xVel/fabs(xVel));
							}
							break;
						case SDLK_UP:
							yVel -= 4;
							if (fabs(yVel) > yVelMax) {
								yVel = yVelMax * (yVel/fabs(yVel));
							}
							break;
						case SDLK_DOWN:
							yVel += 4;
							if (fabs(yVel) > yVelMax) {
								yVel = yVelMax * (yVel/fabs(yVel));
							}
							break;*/
						case SDLK_x:
							camClip = !camClip;
							std::cout << "Switching camera clip behavior" << std::endl;
							break;
						case SDLK_d:
							if (!wasLastD || firstPress) {
								//std::cout << "HO" << std::endl;
								if (firstPress) firstPress = !firstPress;
								diffTime = fpsTimer.getTicks() - lastPress;
								lastPress = fpsTimer.getTicks();
								wasLastD = true;
								didTravel = true;
							}
							break;
						case SDLK_f:
							if (wasLastD || firstPress) {
								//std::cout << "HEAVE" << std::endl;
								if (firstPress) firstPress = !firstPress;
								diffTime = fpsTimer.getTicks() - lastPress;
								lastPress = fpsTimer.getTicks();
								wasLastD = false;
								didTravel = true;
							}
							break;
						default:
							break;
					}
				}
				else if (event.type == SDL_QUIT) {
					exit(0) ;
				}
			}
			std::cout << "\t\t\t" << diffTime << "=dT\n";
			int xBound = SCRN_WIDTH-16;
			int yBound = SCRN_HEIGHT-32;
			if (!raceDone) {
				int otherDiffTime = fpsTimer.getTicks() - lastPress;
				//printf("diffTime = %d\n", diffTime);
				if (diffTime != 0) {
					xVel = 500.0/diffTime;
				} else {
					std::cout << "MAKER'S FDKJFALDJFKL ASJFKLA" << std::endl;
					//xVel = 0;
				}
				if (otherDiffTime > 200 && otherDiffTime%25 == 24) {
					xVel *= .8;
				}
				raceDistance += xVel;

				std::cout << xPos << "=x before velocity\n";

				xPos += xVel;
				std::cout << xVel << "=xV\n";
				yPos += yVel;
				xBound = (SCRN_WIDTH-16)/2;
			}
			else {
				xBound = SCRN_WIDTH+16;
				xPos += xVel;
				//raceDistance += xVel;
			}
			if (xVel > maxVelocity) {
				xVel = maxVelocity;
			}
			if (xPos > xBound || xPos < 0) {
				//xVel = 0;
				if (xPos > xBound) {
					xPos = xBound;
				}
				else {
					xPos = 0;
				}
			}
			if (yPos > yBound || yPos < 0) {
				yVel = 0;
				if (yPos > yBound) {
					yPos = yBound;
				}
				else { yPos = 0; }
			}
			if (xVel > 0 && fpsTimer.getTicks()%framesPerAnimClip == 0) {
				switch(currentFrame) {
					case 0:
						currentFrame = 1;
						previousFrame = 0;
						break;
					case 1:
						if (previousFrame == 0) {
							currentFrame = 2;
						}
						else { currentFrame = 1; }
						previousFrame = 1;
						break;
					case 2:
						currentFrame = 1;
						previousFrame = 2;
						break;
					default:
						currentFrame = 0;
						previousFrame = 0;
				}
			}
		}

		int getCurrentFrame() {
			return currentFrame;
		}

		int getRaceDistance() {
			return raceDistance;
		}

};

#endif
