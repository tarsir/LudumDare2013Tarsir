#ifndef _TIMER_H
#define _TIMER_H

#include "general.h"


const int FPS = 20;
int frame = 0;
bool cap = true;


class Timer {
	private:
		int startTicks;
		int pausedTicks;

		bool paused, started;
	public:
		Timer() {
			startTicks = pausedTicks = 0;
			paused = started = false;
		}

		void start() {
			started = true;
			paused = false;
			startTicks = SDL_GetTicks();
		}
		void stop() {
			started = false;
			paused = false;

		}
		void pause() {
			if (started && !paused) {
				paused = true;
				pausedTicks = SDL_GetTicks() - startTicks;
			}
		}
		void unpause() {
			if (paused) {
				paused = false;
				startTicks = SDL_GetTicks() - pausedTicks;
				pausedTicks = 0;
			}
		}

		int getTicks() {
			if (started) {
				if (paused) {
					return pausedTicks;
				}
				else {
					return SDL_GetTicks() - startTicks;
				}
			}
			return 0;
		}

		bool isStarted() {
			return started;
		}

		bool isPaused() {
			return paused;
		}
};

Timer fpsTimer;

void frameCap() {
	frame++;
	if ((cap) && (fpsTimer.getTicks() < 1000/FPS)) {
		SDL_Delay((1000 - FPS) - fpsTimer.getTicks());
	}
}

#endif
