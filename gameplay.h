#ifndef _GAMEPLAY_H
#define _GAMEPLAY_H

#include "general.h"
//#include "character.h"
//#include "map.h"

extern const int SCRN_HEIGHT;

class GameplayController {
	private:
		int curMap;
		int curState;
	public:
		GameplayController() {
			curMap = 0;
		}

		void changeMap(int amount = 0) {
			if (amount == 0) {
				curMap++;
			}
			else { curMap += amount; }
		}

		void checkForGoalReached(Character* pc, Map* mapdat) {
			int pcX = pc->getXPos(), pcY = pc->getYPos();
			int pcXRight = pcX+16, pcYBottom = pcY+32;
			int goalX = mapdat->getGoalX(), goalY = mapdat->getGoalY();
			int goalXRight = goalX+32, goalYBottom = goalY+32;
			if ((pcXRight > goalX || goalXRight< pcX) && (pcY < goalYBottom || pcYBottom > goalY)) {
				makeText("UR A WINNING", 100, 80);
				//std::cout << "hello" << endl;
			}
		}

		void checkCollisions(Character* pc, Map* mapdat) {
			int pcX = pc->getXPos(), pcY = pc->getYPos();
			int pcXRight = pcX+16, pcYBottom = pcY+32;
			int direction = pc->getDirection();
			bool didCollide = mapdat->locIsCollidable(pcX, pcY);
			if (didCollide) {
				pc->handleCollision(pcX, pcY, direction);
			}
		}

		void levelOver() {
			curState = 2;

		}
};

#endif

