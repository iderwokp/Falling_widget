#include <SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "sdl_wrap.h"
#include "Falling_widget.h"
#include "vec2d.h"

using namespace Iderwok;

std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde);
float grav_avstand(double avstand, float g);
int mouse_x{0};
int mouse_y{0};
bool clearscreen{false};
void EventHandler(SDL_Event event, bool& quit) {//, int ww, int wh) {
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
double sanitize_angle(Point gjeldende, Point sjekke_mot, double angl) {
	if(gjeldende.X == sjekke_mot.X && gjeldende.Y > sjekke_mot.Y) return angl+180;
	//if(gjeldende.X < sjekke_mot.X && gjeldende.Y > sjekke_mot.Y) return angl+270;
	if(gjeldende.X > sjekke_mot.X && gjeldende.Y > sjekke_mot.Y) return angl+2*(180-angl); 
	if(gjeldende.X > sjekke_mot.X && gjeldende.Y < sjekke_mot.Y) return angl;
	if(gjeldende.X < sjekke_mot.X && gjeldende.Y > sjekke_mot.Y) return -1*angl;
	if(gjeldende.X < sjekke_mot.X && gjeldende.Y < sjekke_mot.Y) return angl;
	return angl;

}
int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) {

	SDL_Event event;
	bool quit{false};
    constexpr int windows_width {1300};
    constexpr int windows_height {700};

    SDL_Init(SDL_INIT_VIDEO);

    Sdl_wrap sdlwrap{std::string{"Falling"}, windows_width, windows_height};
    //SDL_Window* window = sdlwrap.window();
    SDL_Renderer* renderer = sdlwrap.renderer();
    constexpr float speed_konstant{0.1};
    constexpr int fwidget_width{30};
    constexpr int fwidget_height{20};
    constexpr int start_fart{0};
    std::vector<Falling_widget> baller;
    //baller.emplace_back("ball.bmp", renderer, Point{750.0, 350.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0, -2, 0);

    
//    baller.emplace_back("ball.bmp", renderer, Point{500.0, 500.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, -start_fart*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{550.0, 400.0}, fwidget_width, fwidget_height, windows_height, windows_width, -start_fart*speed_konstant, 0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{650.0, 400.0}, fwidget_width, fwidget_height, windows_height, windows_width, -start_fart*speed_konstant, -0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{550.0, 500.0}, fwidget_width+10, fwidget_height+10, windows_height, windows_width, -start_fart*speed_konstant, 0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{650.0, 500.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0*speed_konstant, 0*speed_konstant, 0);
    //baller.emplace_back("ball.bmp", renderer, Point{650.0, 500.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, -0*speed_konstant, 0);
  //  baller.emplace_back("ball.bmp", renderer, Point{700.0, 500.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0*speed_konstant, -0*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{630.0, 555.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0*speed_konstant, 0*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{444.0, 333.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0); 
//    baller.emplace_back("ball.bmp", renderer, Point{800.0, 50.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, -0*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{250.0, 100.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{300.0, 70.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0);





 
    int index{1};
    constexpr float gravitasjon{0.0981*speed_konstant*speed_konstant};
    //float gravitasjon2{9.81/1500};
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

        	for(const auto& sjekke_mot_ball: baller) {
        		if(gjeldende_ball == sjekke_mot_ball) continue;

        		Vec2d<double> sjekke_mot_ball_vec{sjekke_mot_ball.current_pos().X, sjekke_mot_ball.current_pos().Y};
        		//acc_vec_deb += sjekke_mot_ball_vec;
        		acc_vec += (sjekke_mot_ball_vec - start_vec);
        	}

        	
        	acc_vec_X = acc_vec.xVal();
        	acc_vec_Y = acc_vec.yVal();
        	
        	double length_acc_vec = acc_vec.length();
	        double justert_lengde = length_acc_vec/10;
	        
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	        SDL_RenderDrawLine(renderer, xPos,yPos, (acc_vec+ start_vec).xVal(), (acc_vec+ start_vec).yVal());
	        //std::cout << "xPos=" << xPos << ", yPos=" << yPos << " -> acc_vec.xVal()=" << acc_vec.xVal() << ", acc_vec.yVal()=" << acc_vec.yVal() << std::endl;

			double anglea = angle_deg(acc_vec, x_axe);
			double angle = sanitize_angle(Point{gjeldende_ball.current_pos().X, gjeldende_ball.current_pos().Y}, Point{acc_vec_X, acc_vec_Y}, anglea);
		    std::cout << "anglea = " << anglea << "\tangle = " << angle << std::endl;
	        float grav_rr = grav_avstand(justert_lengde, gravitasjon);
	        gjeldende_ball.set_aksellerasjon(grav_rr, static_cast<int>(angle));
	        gjeldende_ball.updateXY();
	        index--;
        }

Sdl_wrap();
//anglea = 45     		  angle = 315
//anglea = 135.144        angle = 224.856
//anglea = 45.4286        angle = 314.571
//anglea = 134.716        angle = 225.284

		SDL_RenderPresent(renderer);
		//SDL_RenderClear(renderer);
		SDL_Delay(10);



    }

    SDL_Quit();
    return 0;
}

std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde) {

	double x = lengde*cos(rad_vinkel);
	double y = lengde*sin(rad_vinkel);
	return std::make_pair(x+startX, y+startY);

}

float grav_avstand(double avstand, float g) {
	if(avstand< 5.0f) avstand = 5.0f;
	//return g/(avstand*avstand);
	return g;
}



