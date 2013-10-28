#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>
#include <sstream>
#include <cmath>

#include "map.h"
#include "general.h"
#include "graphics.h"
#include "character.h"
#include "timer.h"
#include "gameplay.h"
#include "scroll.h"

enum {
	GAMESTAGE,
	START,
	SCORE,
	MAINMENU,
	LOADBUMBLE,
	OPTIONSMENU,
	DUDERSCREEN,
	LEAVE,
	CREDITS,
	DEATH
};


using namespace std;

int curState = START;

void showPreLevelScreen(int lvl) {

}



int main ( int argc, char** argv )
{
	init();
	initGfx();

	addFilesToLists();

	Map* cur = new Map("part1");
	Camera* mainCam = new Camera();
	ImgRsrc* duder1 = new ImgRsrc("test");
	ImgRsrc* duder2 = new ImgRsrc("test2");
	ImgRsrc* duder3 = new ImgRsrc("test3");
	ImgRsrc* background = findTileByLabel("background");
	ImgRsrc* greything = findTileByLabel("greything");
	ImgRsrc* scoreScreen = findTileByLabel("score");
	ImgRsrc* splash = findTileByLabel("splash");
	ImgRsrc* cursor = findTileByLabel("cursor");
	ImgRsrc* mainmenu = findTileByLabel("runnyguymain");
	ImgRsrc* star = findTileByLabel("star");
	ImgRsrc* bigDuder = findSpriteByLabel("bigduder");
	ImgRsrc* howplay = findTileByLabel("howplay");


	ImgRsrc* prelvl1 = findTileByLabel("prelvl1");
	ImgRsrc* prelvl2 = findTileByLabel("prelvl2");
	ImgRsrc* prelvl3 = findTileByLabel("prelvl3");
	ImgRsrc* prelvl4 = findTileByLabel("prelvl4");
	//ImgRsrc* stage1 = findTileByLabel("stage1");
	//ImgRsrc* stage3 = findTileByLabel("stage3");
	//ImgRsrc* stage4 = findTileByLabel("stg4");

	//ImgRsrc* dirt = findTileByLabel("dirttrack");
	ImgRsrc* goal = findTileByLabel("goal");

	ImgRsrc* endscreen = findTileByLabel("endscreen");
	ImgRsrc* credits = findTileByLabel("credits");
	PlayerCharacter* guyman = new PlayerCharacter("dudeman", duder1);
	GameplayController* gpControl = new GameplayController();

	Background* skybox = new Background("background");
	Background* dirt = new Background("dirttrack", 0, 380);
	Background* dirt2 = new Background("dirttrack2", 0, 370);
	Background* stg3 = new Background("stage3");
	Background* stg1 = new Background("stage1");
	Background* stg1track = new Background("track1", 0, 320);
	Background* stg4 = new Background("stg4");

	skybox->setXScrollRate(2);

	mainCam->setTarget(guyman);
    // program main loop
    bool done = false;
    bool canInput = false;
    guyman->setPosition(0, 361);
    int startTime = -1;
    stringstream str;
    int xOne = 0, xTwo = guyman->getXPos(), temp;
    double time = 1000;
    bool raceStarted = false;
    bool doneskies = false, didTravel = false;
    SDL_Surface* curDuderFrame;
    int cursorPos = 0;
    int level = 0;
    int countedScore = 0;
    bool doneCounting = false;
    bool delayOnce = false;
    int maxScore, numStars, achievedScore;
    bool stateChanged = false;
    Background* currentBg, *currentTrack;

    while (!done)
	{
		switch(curState) {
			case START:
			{
				applySurface(splash->getSfc(), screen, 0, 0);
				curState = MAINMENU;
				refresh();
				SDL_Delay(3000);
				break;
			}
			case MAINMENU:
			{
				if (stateChanged) {
					cursorPos = 0;
					stateChanged = false;
				}
				applySurface(mainmenu->getSfc(), screen, 0, 0);
				makeText("NEW", 40, 340);
				makeText("LOAD", 40, 400);
				makeText("HOW PLAY", 340, 340);
				makeText("RUN AWAY", 340, 400);
				if (cursorPos == 0) {
					applySurface(cursor->getSfc(), screen, 5, 358);
				}
				else if (cursorPos == 1) {
					applySurface(cursor->getSfc(), screen, 5, 418);
				}
				else if (cursorPos == 2) {
					applySurface(cursor->getSfc(), screen, 305, 358);
				}
				else if (cursorPos == 3) {
					applySurface(cursor->getSfc(), screen, 305, 418);
				}
				refresh();
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_KEYDOWN) {
						switch (event.key.keysym.sym) {
							case SDLK_LEFT:
								if (cursorPos == 2 || cursorPos == 3)
									cursorPos -= 2;
								break;
							case SDLK_RIGHT:
								if (cursorPos == 0 || cursorPos == 1)
									cursorPos += 2;
								break;
							case SDLK_UP:
								if (cursorPos == 1 || cursorPos == 3)
									cursorPos -= 1;
								break;
							case SDLK_DOWN:
								if (cursorPos == 0 || cursorPos == 2)
									cursorPos += 1;
								break;
							case SDLK_RETURN:
								stateChanged = true;
								switch(cursorPos) {
									case 0:
										level = 0;
										curState = GAMESTAGE;
										break;
									case 1:
										curState = LOADBUMBLE;
										break;
									case 2:
										curState = OPTIONSMENU;
										break;
									case 3:
										curState = LEAVE;
										break;
									default:
										std::cout << "WHAT THE FUCK DID YOU DO" << std::endl;
										exit(1);
								}
								cursorPos = 0;
								break;
						}
					}
					if (event.type == SDL_QUIT) {
						exit(1);
					}
				}
				break;
			}
			case GAMESTAGE:
			{
				numStars = 0;
				if (stateChanged) {
					stateChanged = false;
					//std::cout << "startTime=" << startTime << std::endl;
					doneskies = false;
					raceStarted = false;
					canInput = false;
					guyman->setPosition(0, 361);
					guyman->setXVelocity(0);
					guyman->resetDistance();
					//showPreLevelScreen(level);

					ImgRsrc* lvlShow;
					switch(level) {
						case 0:
							lvlShow = prelvl1;
							break;
						case 1:
							lvlShow = prelvl2;
							break;
						case 2:
							lvlShow = prelvl3;
							break;
						case 3:
							lvlShow = prelvl4;
							break;
						default:
							lvlShow = prelvl1;
					}
					applySurface(lvlShow->getSfc(), screen, 0, 0);
					refresh();


					switch (level) {
						case 0:
							guyman->setPosition(0, 361);
							currentBg = stg1;
							currentTrack = stg1track;
							currentBg->setXScrollRate(2);
							currentTrack->setXScrollRate(2);
							break;
						case 1:
							guyman->setPosition(0, 361);
							currentBg = skybox;
							currentTrack = dirt;
							break;
						case 2:
							guyman->setPosition(0, 406);
							currentBg = stg3;
							currentTrack = dirt2;
							currentBg->setXScrollRate(2);
							break;
						case 3:
							guyman->setPosition(0, 361);
							currentBg = stg4;
							currentTrack = dirt;
							currentBg->setXScrollRate(2);
							break;
						case 4:
							curState = CREDITS;
							stateChanged = true;
							break;
					}
					SDL_Delay(5000);
					startTime = fpsTimer.getTicks();
				}
				/*if (startTime <= 0 && !raceStarted && !doneskies) {
					startTime = fpsTimer.getTicks();
					std::cout << "AHHH" << std::endl;
				}*/
				//std::cout << guyman->getXPos() << "=x" << std::endl;
				currentTrack->setXScrollRate(guyman->getXVelocity());
				int now = fpsTimer.getTicks();
				int difference = now - startTime;
				std::cout << "difference=" << difference << std::endl;

				if (!doneskies) time = (13000 - difference)/1000.0;
				//std::cout << "time=" << time << std::endl;
				//std::cout << "time2=" << now - startTime << std::endl;
				if (canInput) {
					guyman->update(doneskies, didTravel);
				}
				//else if (time == 1000) { time = 10; }

				if (!raceStarted) {
					time = 10;
				}
				if (time <= 0) {
					time = 0;
					canInput = false;
					//gpControl->levelOver();
					doneskies = true;
					guyman->update(doneskies, didTravel);
					startTime = -1;
					//curState = SCORE;
				}

				str.str("");
				//std::cout << didTravel << "=travelled\n";
				canInput ? 1 : didTravel = false;
				str.precision(3);
				str << fixed << time;
				//gpControl->checkCollisions(guyman, cur);
				// DRAWING STARTS HERE

				// clear screen
				//cout << "Clearing screen!" << endl;
				SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

				// draw stuff
				mainCam->centerCameraOnTarget();
				mainCam->keepInBounds(99999, 99999);
				SDL_Rect camrect = mainCam->getCam();
				//applySurface(background->getSfc(), screen, 0, 0);
				temp = xTwo;
				xTwo = guyman->getXPos();
				xOne = temp;
				currentBg->update(didTravel, false);
				currentTrack->update(didTravel, false);
				//applySurface(goal->getSfc(), screen, 624, 380);
				if (1) {
					if (time == 10) { str.str("10"); }
					string timeString = "Time: ";
					timeString += str.str().c_str();
					if (!makeText(timeString, 20, 20)) {
						//cout << "Couldn't make text happen" << endl;
					}
				}
				//cur->display(camrect);
				//printf("Camera at: %d, %d\nMax is: %d, %d\n", camrect.x, camrect.y, cur->getXBound(), cur->getYBound());
				//printf("Duder at: %d, %d\n", guyman->getXPos(), guyman->getYPos());
				switch (guyman->getCurrentFrame()) {
					case 1: curDuderFrame = duder1->getSfc();
						break;
					case 2: curDuderFrame = duder2->getSfc();
						break;
					case 3: curDuderFrame = duder3->getSfc();
						break;
					default: curDuderFrame = duder1->getSfc();
				}
				applySurface(curDuderFrame, screen, guyman->getXPos(), guyman->getYPos());
				if (doneskies) {
					//makeText("STOP!", 300, 0);
					str.str("");
					str << guyman->getRaceDistance();
					//makeText(str.str(), 150, 0);
				}
				//cout << "Putting guy on screen!" << endl;

				// DRAWING ENDS HERE

				// finally, update the screen :)
				//gpControl->checkForGoalReached(guyman, cur);
				if (now - startTime < 1000) {
					makeText("3...", 100, 200);
				}
				else if (now - startTime < 2000) {
					makeText("2...", 100, 200);
				}
				else if (now - startTime < 3000) {
					makeText("1...", 100, 200);
				}
				else if (now - startTime < 4500) {
					makeText("GO!!!", 100, 200);
					canInput = true;
					raceStarted = true;
				}
				refresh();
				frameCap();
				if (doneskies && (guyman->getXPos()>SCRN_WIDTH || guyman->getXVelocity() == 0)) {
					SDL_Delay(1000);
					curState = SCORE;
					stateChanged = true;
				}
				break;
			}
			case SCORE:
			{
			    int cursorMin = 0;
				if (stateChanged) {
                    cursorPos = 0;
					delayOnce = false;
					countedScore = 0;
					doneCounting = false;
					stateChanged = false;
				}
				maxScore = 5500;
				achievedScore = guyman->getRaceDistance() * 1.2;
				std::cout << "maxScore = " << maxScore << std::endl;
				//maxScore = achievedScore;
				applySurface(scoreScreen->getSfc(), screen, 0, 0);
				applySurface(bigDuder->getSfc(), screen, 71, 294);
				makeText("SCORE", 177, 38);
				countedScore += 17;
				if (countedScore > maxScore/5) {
					numStars = 1;
				}
				if (countedScore > .3 * maxScore) {
					numStars = 2;
				}
				if (countedScore > .5 * maxScore) {
					numStars = 3;
				}
				if (countedScore > .75 * maxScore) {
					numStars = 4;
				}
				if (countedScore > .99 * maxScore) {
					numStars = 5;
				}
				for (int i = 0; i < numStars; i++) {
					applySurface(star->getSfc(), screen, 366 + 38*i, 171);
				}
				if (countedScore >= achievedScore) {
					doneCounting = true;
					countedScore = achievedScore;
				}
				str.str("");
				str << countedScore;
				makeText(str.str(), 61, 160);
				if (doneCounting) {
					int cursorY = 0, cursorX = 0;
					if (!delayOnce) {
						refresh();
						SDL_Delay(1500);
					}
					delayOnce = true;
					if (numStars > 2) {
						makeText("Next", 237, 290);
						cursorMin = 0;
					} else {
                        cursorMin = 1;
                    }
					makeText("Retry", 237, 410);
					//makeText("Duder", 237, 350);
					makeText("Menu", 237, 350);
					cursorX = 202;
					if (cursorPos < cursorMin) cursorPos = cursorMin;
					switch (cursorPos) {
						case 0:
							cursorY = 308;
							break;
						case 1:
							cursorY = 368;
							break;
						case 2:
							cursorY = 428;
							break;
						/*case 3:
							cursorY = 368;
							cursorX = 465;
							break;*/
					}
					applySurface(cursor->getSfc(), screen, cursorX, cursorY);
					while (SDL_PollEvent(&event)) {
						if (event.type == SDL_KEYDOWN) {
							switch (event.key.keysym.sym) {
								case SDLK_UP:
									if (cursorPos > 0) {
                                        if (numStars > 2 || cursorPos == 2)
                                            cursorPos--;
									}
									break;
								case SDLK_DOWN:
									if (cursorPos < 2) {
										cursorPos++;
									}
									break;
								/*case SDLK_RIGHT:
									cursorPos = 3;
									break;
								case SDLK_LEFT:
									cursorPos = 1;
									break;*/
								case SDLK_RETURN:
									switch (cursorPos) {
										case 0:
											level++;
											curState = GAMESTAGE;
											break;
										case 1:
											curState = MAINMENU;
											break;
										case 2:
											curState = GAMESTAGE;
											break;
										case 3:
											curState = MAINMENU;
											break;
									}
									stateChanged = true;
									break;
							}
						}
						else if (event.type == SDL_QUIT) {
							exit(1);
						}
					}
				}
				refresh();
				SDL_Delay(5);
				frameCap();
				break;
			}
			case LOADBUMBLE:
			{
				if (stateChanged) {
					cursorPos = 0;
					stateChanged = false;
				}
				int cursorX = 5, cursorY = 358;
				applySurface(mainmenu->getSfc(), screen, 0, 0);
				makeText("Level 1", 40, 340);
				makeText("Level 2", 40, 400);
				makeText("Level 3", 340, 340);
				makeText("Level 4", 340, 400);
				if (cursorPos == 0) {
					//do nothing
				} else if (cursorPos == 1) {
					cursorY = 418;
				} else if (cursorPos == 2) {
					cursorX = 305;
					cursorY = 358;
				} else if (cursorPos == 3) {
					cursorX = 305;
					cursorY = 418;
				}
				applySurface(cursor->getSfc(), screen, cursorX, cursorY);
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_KEYDOWN) {
						switch (event.key.keysym.sym) {
							case SDLK_LEFT:
								if (cursorPos > 1) {
									cursorPos -= 2;
								}
								break;
							case SDLK_RIGHT:
								if (cursorPos < 2) {
									cursorPos += 2;
								}
								break;
							case SDLK_UP:
								if (cursorPos%2 == 1) {
									cursorPos--;
								}
								break;
							case SDLK_DOWN:
								if (cursorPos%2 == 0) {
									cursorPos++;
								}
								break;
							case SDLK_RETURN:
								stateChanged = true;
								curState = GAMESTAGE;
								level = cursorPos;
								cursorPos = 0;
								break;
							case SDLK_ESCAPE:
								stateChanged = true;
								curState = MAINMENU;
								cursorPos = 0;
								break;
						}
					}
					else if (event.type == SDL_QUIT) {
						exit(1);
					}
				}
				refresh();
				break;
			}

			case OPTIONSMENU:
			{
				applySurface(howplay->getSfc(), screen, 0, 0);
				refresh();
				SDL_Delay(1000);
				curState = MAINMENU;
				stateChanged = true;
				break;
			}

			case LEAVE:
			{
				applySurface(endscreen->getSfc(), screen, 0, 0);
				refresh();
				SDL_Delay(5000);
				exit(1);
			}

			case CREDITS:
			{
				applySurface(credits->getSfc(), screen, 0, 0);
				refresh();
				SDL_Delay(1000);
				curState = MAINMENU;
				stateChanged = true;
				break;
			}
		}
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(duder1->getSfc());

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
