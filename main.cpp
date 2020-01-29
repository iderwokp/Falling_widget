//#include <SDL.h>
//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <array>
//#include "sdl_wrap.h"
//#include "Falling_widget.h"
//#include "vec2d.h"
//#include "ammo.h"
//#include "asteroids.h"

#include "asteroid_game.h"
using namespace Iderwok;


int main(int argc, char** argv) {
	Asteroid_game ag("Asteroids", 1300, 700);
	ag.start_game();

    return 0;
}

