#include <SDL.h>
#include <iostream>
#include <cmath>
#include "sdl_wrap.h"
#include "Falling_widget.h"

std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde);

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
    Falling_widget fw("ball.bmp", renderer, Point{100.0, 100.0}, 30, 20, windows_height, windows_width, 0, 2, 0);
//    float aksy = 0.98f;
//    float aksx = 0.98f;
//    fw.set_aksellerasjon(aksx, aksy);
    
    const double midwinX = windows_width/2;
    const double midwinY = windows_height/2;
    int index{300};
    
    while(index >=0 && !quit) {
	
        EventHandler(event, quit, windows_width, windows_height);
        //fw.updateXY();
        int ypos = fw.current_pos().Y;
        int xpos = fw.current_pos().X;
        
        double vecX = midwinX - xpos;//static_cast<double>(x);
        double vecY = midwinY - ypos;//static_cast<double>(y);
        double length_vecXY = sqrt(vecX*vecX + vecY*vecY);
        
        double ekspr = (vecX*vecX+vecY*0)/(length_vecXY*vecX);
        double radangl = acos(ekspr);
        if (ypos > midwinY) radangl *= -1;
        int angle = static_cast<int>(radangl*(180.0/3.1415926));
        fw.set_aksellerasjon(9.81, angle);
        auto [endX, endY] = endKoord(xpos, ypos, radangl, length_vecXY);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer); 
	    
	    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        
		SDL_RenderDrawLine(renderer, 0,midwinY, windows_width,midwinY);
		SDL_RenderDrawLine(renderer, midwinX,0, midwinX, windows_height);
		SDL_RenderDrawLine(renderer, xpos,ypos, endX, endY);
		fw.updateXY();
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
