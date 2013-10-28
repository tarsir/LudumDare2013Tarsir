#ifndef _MAP_H_
#define _MAP_H_

#include "general.h"
#include "graphics.h"
#include <vector>
#include <fstream>

using std::vector;
using std::ifstream;
using std::string;
using std::cout;
using std::endl;

string characterToImgLabel(char inp);
bool characterToCollidable(char inp);
extern bool camClip;

class Map {
	private:
		string name;
		int xMax, yMax;
		int goalX, goalY;
		vector< string > tileMatrix;
		vector< vector< bool> > collisionMap;
	public:
		Map(char* inpFile) {
			goalX = 624, goalY = 380;
			xMax = yMax = 0;
			char* fullpath = new char[300];
			strcpy(fullpath, "assets/maps/");
			strcat(fullpath, inpFile);
			cout << "Looking for map at " << fullpath << endl;
			try {
				string line;
				int count = 0;
				ifstream mapFile (fullpath);
				if (mapFile.is_open()) {
					while (mapFile.good()) {
						vector<bool> row;
						getline (mapFile, line);
						cout << line << endl;
						int loc = line.find('X');

						for (string::iterator it = line.begin(); it < line.end(); it++) {
							row.push_back(characterToCollidable((*it)));
							cout << row.back();
						}
						cout << endl;

						collisionMap.push_back(row);
						tileMatrix.push_back(line);
						count++;
					}
					mapFile.close();
				}
				printf("Goal located at: %d, %d\n", goalX, goalY);

			}
			catch (...) {
				std::cout << "Couldn't find the map file..." << endl;
			}
			yMax = tileMatrix.size() * 32;
			xMax = tileMatrix[0].length() * 32;
			cout << collisionMap.size() << endl;
		}

		void display(SDL_Rect camera) {
			int counter = 0;
			SDL_Rect* camPtr;
			if (!camClip) {
				camPtr = NULL;
			}
			else { camPtr = &camera; }
			for (vector<string>::iterator it = tileMatrix.begin(); it < tileMatrix.end(); it++) {
				for (int i = 0; i < xMax; i++) {
					//std::cout << (*it)[i];
					string tileLabel = characterToImgLabel((*it)[i]);
					ImgRsrc* tileRsrc = findTileByLabel(tileLabel);
					//std::cout << tileLabel << endl;
					applySurface(tileRsrc->getSfc(), screen, i*32, counter*32, camPtr);
				}
				//std::cout << endl;
				counter++;
			}
		}

		bool locIsCollidable(int x, int y) {
			collisionMap.empty();
			int tileX = x / 32, tileY = y / 32;
			int tileXRight = (x + 16)/32, tileYBot = (y + 32)/32;
			//printf("Checking collisions between: %d, %d ;; %d, %d\n", tileX, tileY, tileXRight, tileYBot);
			//printf("Size of collision map: %d rows, %d columns", collisionMap.size(), collisionMap[0].size());
			std::cout << endl;
			return collisionMap[tileY][tileX] || collisionMap[tileYBot][tileXRight];
		}


		int getXBound() { return xMax; }
		int getYBound() { return yMax; }
		int getGoalX() { return goalX; }
		int getGoalY() { return goalY; }
};

string characterToImgLabel(char inp) {
	switch (inp) {
		case 'W':
			return "water";
			break;
		case 'G':case 'g':
			return "grass";
			break;
		case 'B':
			return "brick";
			break;
		default:
			return "sunshinetile";
	}
}

bool characterToCollidable(char inp) {
	switch (inp) {
		case 'W':
		case 'B':
			return true;
		default:
			return false;
	}
}

#endif
