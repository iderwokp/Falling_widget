#include <SDL.h>
#include <iostream>
#include <cmath>
#include "sdl_wrap.h"
#include "Falling_widget.h"

std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde);
float grav_avstand(double avstand, float g);

void EventHandler(SDL_Event event, bool& quit, int ww, int wh) {
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT){
        quit = true;
    }    
//    if(event.type == SDL_MOUSEBUTTONDOWN) {
//        int x{0};
//        int y{0};
//        SDL_GetMouseState( &x, &y );
//        check_mouse_click(x, y, w, ww, wh);
//        
//    }
}
int main(int argc, char** argv) {
	
	SDL_Event event;
	bool quit{false};
    const int windows_width {1300};
    const int windows_height {700};
    SDL_Init(SDL_INIT_VIDEO);
    
    Sdl_wrap sdlwrap{std::string{"Falling"}, windows_width, windows_height};
    SDL_Window* window = sdlwrap.window();
    SDL_Renderer* renderer = sdlwrap.renderer();
    bool nedovery{true};
    bool nedoverx{true};
    //Falling_widget(std::string fn, SDL_Renderer* rend, Point p={0, 0}, int w = 0, int h = 0, int bound, int wbound,double vx0 = 0, double vy0 = 0,int rot= 0)
    Falling_widget fw("ball.bmp", renderer, Point{350.0, 350.0}, 30, 20, windows_height, windows_width, 0, 2.5, 0);
    Falling_widget fw2("ball.bmp", renderer, Point{850.0, 350.0}, 30, 20, windows_height, windows_width, 0, -2.5, 0);
//    float aksy = 0.98f;
//    float aksx = 0.98f;
//    fw.set_aksellerasjon(aksx, aksy);
    
    const double midwinX = windows_width/2;
    const double midwinY = windows_height/2;
    int index{300};
    float gravitasjon{9.81/100};
    while(index >=0 && !quit) {
	
        EventHandler(event, quit, windows_width, windows_height);
        //fw.updateXY();
        int ypos = fw.current_pos().Y;
        int xpos = fw.current_pos().X;
        int ypos2 = fw2.current_pos().Y;
        int xpos2 = fw2.current_pos().X;
        
        double vecX = midwinX - xpos;
        double vecY = midwinY - ypos;
        double vecX2 = midwinX - xpos2;
        double vecY2 = midwinY - ypos2;
        double length_vecXY = sqrt(vecX*vecX + vecY*vecY);
        double length_vecXY2 = sqrt(vecX2*vecX2 + vecY2*vecY2);
        double justert_lengde = length_vecXY/100;
        double justert_lengde2 = length_vecXY2/100;
        
        double ekspr = (vecX*vecX+vecY*0)/(length_vecXY*vecX);
        double ekspr2 = (vecX2*vecX2+vecY2*0)/(length_vecXY2*vecX2);
        double radangl = acos(ekspr);
        double radangl2 = acos(ekspr2);
        if (ypos > midwinY) radangl *= -1;
        if (ypos2 > midwinY) radangl2 *= -1;
        
        int angle = static_cast<int>(radangl*(180.0/3.1415926));
        int angle2 = static_cast<int>(radangl2*(180.0/3.1415926));
        
        float grav_rr = grav_avstand(justert_lengde, gravitasjon);
        float grav_rr2 = grav_avstand(justert_lengde2, gravitasjon);
        //std::cout << "grav_rr = " << grav_rr << "\tjustert_lengde^2 = " << justert_lengde * justert_lengde<<"\n";
        fw.set_aksellerasjon(grav_rr, angle);
        fw2.set_aksellerasjon(grav_rr2, angle2);
        
        auto [endX, endY] = endKoord(xpos, ypos, radangl, length_vecXY);
        auto [endX2, endY2] = endKoord(xpos2, ypos2, radangl2, length_vecXY2);
        
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer); 
	    
	    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        
		SDL_RenderDrawLine(renderer, 0,midwinY, windows_width,midwinY);
		SDL_RenderDrawLine(renderer, midwinX,0, midwinX, windows_height);
		SDL_RenderDrawLine(renderer, xpos,ypos, endX, endY);
		SDL_RenderDrawLine(renderer, xpos2,ypos2, endX2, endY2);
		fw.updateXY();
		fw2.updateXY();
		SDL_RenderPresent(renderer);
		SDL_Delay(30); 
       
       
       
    }
   
    SDL_Quit();
}

std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde) {
	
	double x = lengde*cos(rad_vinkel);
	double y = lengde*sin(rad_vinkel);
	return std::make_pair(x+startX, y+startY);
	
}

float grav_avstand(double avstand, float g) {
	if(avstand< 1.0f) avstand = 1.0f;
	return g/(avstand*avstand);
	
}
