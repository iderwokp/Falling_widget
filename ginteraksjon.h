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
		Gravity_interaksjon(std::string navn, int ww, int wh) : windows_width{ww}, windows_height{wh} {
			SDL_Init(SDL_INIT_VIDEO);
			Sdl_wrap sdlwrap{navn, windows_width, windows_height};
    		//SDL_Window* window = sdlwrap.window();
  		    renderer = sdlwrap.renderer();
  		    init_baller();
  			start();
		
		}
		void start();
		double sanitize_angle(Point gjeldende, Point sjekke_mot, double angl);
		void EventHandler(SDL_Event event, bool& quit);
		std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde);
		float grav_avstand(double avstand, float g);
	private:
		std::vector<Falling_widget> baller{};
		void init_baller();
		int mouse_x{0};
		int mouse_y{0};
		bool clearscreen{false};
		SDL_Event event;
		bool quit{false};
	    const int windows_width {};
	    const int windows_height {};
	    
		SDL_Renderer* renderer;
	    
	    const float speed_konstant{0.1};
	    const int fwidget_width{30};
	    const int fwidget_height{20};
	    const int start_fart{35};
	    const double gravitasjon{14*9.81*speed_konstant*speed_konstant};
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


std::pair<double, double> Gravity_interaksjon::endKoord(double startX, double startY, double rad_vinkel, double lengde) {

	double x = lengde*cos(rad_vinkel);
	double y = lengde*sin(rad_vinkel);
	return std::make_pair(x+startX, y+startY);

}

float Gravity_interaksjon::grav_avstand(double avstand, float g) {
	if(avstand< 5.0f) avstand = 5.0f;
	std::cout << "g = " << g << "\n";
	return g/(avstand*avstand);
	//return g;
}

void Gravity_interaksjon::start() {
	std::cout << "start()\n";
	int index{1};
    
    
    while(index >=0 && !quit) {

        EventHandler(event, quit);//, windows_width, windows_height);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        if(clearscreen) {
        	SDL_RenderClear(renderer);
			clearscreen = false;
        }
        SDL_RenderClear(renderer);
        Vec2d<double> x_axe{500.0, 0.0}; 				//Vektor som representerer x-aksen. Vinkelen er mellom x_axe og vecXY
        for(auto& gjeldende_ball: baller) {
        	double xPos = gjeldende_ball.current_pos().X;
        	double yPos = gjeldende_ball.current_pos().Y;
        	Vec2d<double> acc_vec{0.0, 0.0};
        	//Vec2d<double> acc_vec_deb{0.0, 0.0};
        	Vec2d<double> start_vec {xPos, yPos};

	        double acc_vec_Y{0};
	        double acc_vec_X{0};
			//std::cout << " --------------------------------------------------------------------------------- " << std::endl;
        	for(const auto& sjekke_mot_ball: baller) {
        		if(gjeldende_ball == sjekke_mot_ball) continue;

        		Vec2d<double> sjekke_mot_ball_vec{sjekke_mot_ball.current_pos().X, sjekke_mot_ball.current_pos().Y};
        		Vec2d<double> temp_vec = sjekke_mot_ball_vec - start_vec;
        		double r = temp_vec.length();
        		if(r<10) r = 10;
        		double temp_x = temp_vec.xVal();
        		double temp_y = temp_vec.yVal();

        		double temp_angle = angle_deg(temp_vec, x_axe);
        		double temp_angle_sanitized = sanitize_angle(Point{gjeldende_ball.current_pos().X, gjeldende_ball.current_pos().Y}, Point{temp_x, temp_y}, temp_angle);
        		temp_vec = {40000.0/(r*r), static_cast<int> (temp_angle_sanitized)};
        	
        		acc_vec += (temp_vec);
        	}
//DONE: Avstanden mellom to objekter bør bestemme magnituden på vektorene, og ikke bare på resultant vektoren.
//TODO: Sette individuell gravitasjon
        	acc_vec_X = acc_vec.xVal();
        	acc_vec_Y = acc_vec.yVal();

        	double length_acc_vec = acc_vec.length();
	        double justert_lengde = length_acc_vec/10;

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	        SDL_RenderDrawLine(renderer, xPos,yPos, (acc_vec+ start_vec).xVal(), (acc_vec+ start_vec).yVal());
	        //std::cout << "xPos=" << xPos << ", yPos=" << yPos << " -> acc_vec.xVal()=" << acc_vec.xVal() << ", acc_vec.yVal()=" << acc_vec.yVal() << std::endl;

			double anglea = angle_deg(acc_vec, x_axe);
			double angle = sanitize_angle(Point{gjeldende_ball.current_pos().X, gjeldende_ball.current_pos().Y}, Point{acc_vec_X, acc_vec_Y}, anglea);
		    //std::cout << "anglea = " << anglea << "\tangle = " << angle << std::endl;
	        float grav_rr = grav_avstand(justert_lengde, gravitasjon);
	        gjeldende_ball.set_aksellerasjon(grav_rr, static_cast<int>(angle));
	        gjeldende_ball.updateXY();
	        //index--;
        }

//Sdl_wrap()
//anglea = 45     		  angle = 315
//anglea = 135.144        angle = 224.856
//anglea = 45.4286        angle = 314.571
//anglea = 134.716        angle = 225.284

		SDL_RenderPresent(renderer);
		//SDL_RenderClear(renderer);
		SDL_Delay(10);



    }

	
}
void Gravity_interaksjon::init_baller() {
	
	baller.emplace_back("ball.bmp", renderer, Point{250.0, 200.0}, fwidget_width, fwidget_height, windows_height, windows_width, start_fart*speed_konstant, 0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{650.0, 400.0}, fwidget_width, fwidget_height, windows_height, windows_width, -start_fart*speed_konstant, -start_fart*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{550.0, 600.0}, fwidget_width, fwidget_height, windows_height, windows_width, start_fart*speed_konstant, 0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{150.0, 550.0}, fwidget_width, fwidget_height, windows_height, windows_width, start_fart*speed_konstant, start_fart*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{850.0, 200.0}, fwidget_width, fwidget_height, windows_height, windows_width, -start_fart*speed_konstant, -0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{700.0, 500.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0*speed_konstant, -0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{630.0, 555.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0*speed_konstant, 0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{444.0, 333.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{800.0, 50.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, -0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{250.0, 100.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{300.0, 70.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0);
	
	
}

} //namespace
#endif

