#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "sdl_wrap.h"
#include "Falling_widget.h"
#include "vec2d.h"
#include "ginteraksjon.h"

using namespace Iderwok;

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) {



	Gravity_interaksjon gi(std::string("Falling"), 1300, 600);


    SDL_Quit();
    return 0;
}


