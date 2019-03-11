#ifndef GINTERAKSJON_H
#define GINTERAKSJON_H

#include <vector>
#include <cmath>
#include "sdl_wrap.h"
#include "Falling_widget.h"
#include "vec2d.h"

namespace Iderwok {
class  Gravity_interaksjon{
	
	public:
		Gravity_interaksjon(const std::vector<Falling_widget> fw, int ww, int wh) : fwidget{fw}, windows_width{ww}, windows_height{wh} {
			SDL_Init(SDL_INIT_VIDEO);
		
		}
		void start();
		double sanitize_angle(Point gjeldende, Point sjekke_mot, double angl);
		void EventHandler(SDL_Event event, bool& quit);
		std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde)
	private:
		std::vector<Falling_widget> fwidget_{};
		int mouse_x{0};
		int mouse_y{0};
		bool clearscreen{false};
		SDL_Event event;
		bool quit{false};
	    constexpr int windows_width {};
	    constexpr int windows_height {};
};

double Gravity_interaksjon::sanitize_angle(Point gjeldende, Point sjekke_mot, double angl) {
	if(gjeldende.X == (gjeldende.X+sjekke_mot.X) && gjeldende.Y > (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 0 << std::endl;*/return angl+180;}
	//if(gjeldende.X < sjekke_mot.X && gjeldende.Y > sjekke_mot.Y) return angl+270;
	if(gjeldende.X > (gjeldende.X+sjekke_mot.X) && gjeldende.Y > (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 1 << std::endl; */return 180+(180-angl);}//+2*(180-angl);
	if(gjeldende.X > (gjeldende.X+sjekke_mot.X) && gjeldende.Y < (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 2 << std::endl; */return angl;}
	if(gjeldende.X < (gjeldende.X+sjekke_mot.X) && gjeldende.Y > (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 3 << std::endl;*/ return 360-angl;}
	if(gjeldende.X < (gjeldende.X+sjekke_mot.X) && gjeldende.Y < (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 4 << std::endl; */return angl;}
	return angl;

}

void Gravity_interaksjon::EventHandler(SDL_Event event, bool& quit) {//, int ww, int wh) {
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT){
        quit = true;
    }
    if(event.type == SDL_MOUSEBUTTONDOWN) {
        clearscreen = true;
        SDL_GetMouseState( &mouse_x, &mouse_y );
        //check_mouse_click(x, y, w, ww, wh);

    }
}

void Gravity_interaksjon::start() {
	
	
}

std::pair<double, double> Gravity_interaksjon::endKoord(double startX, double startY, double rad_vinkel, double lengde) {

	double x = lengde*cos(rad_vinkel);
	double y = lengde*sin(rad_vinkel);
	return std::make_pair(x+startX, y+startY);

}

float Gravity_interaksjon::grav_avstand(double avstand, float g) {
	if(avstand< 5.0f) avstand = 5.0f;
	return g/(avstand*avstand);
	//return g;
}


} //namespace
#endif

