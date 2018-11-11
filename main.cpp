#include <SDL.h>
#include <iostream>
#include "sdl_wrap.h"
#include "Falling_widget.h"


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
    Falling_widget fw("ball.bmp", renderer, Point{10.0, 100.0}, 30, 20, windows_height, windows_width, 0, 0, 0);
    float aksy = 0.98f;
    float aksx = 0.98f;
    fw.set_aksellerasjon(aksx, aksy);
    int index{300};
    
    while(index >=0 && !quit) {
	
        EventHandler(event, quit, windows_width, windows_height);
        fw.updateXY();
        int ypos = fw.current_pos().Y;
        int xpos = fw.current_pos().X;
        
        SDL_RenderPresent(renderer);
        //SDL_RenderClear(renderer);  
		SDL_Delay(15); 
        if(nedovery == true && ypos >= windows_height/2  ) {
			//fw.set_aksellerasjon(aksx, -aksy);
			aksy *=-1;
			nedovery = false;
			
		}
       if(nedovery == false && ypos <= windows_height/2  ) {
			//fw.set_aksellerasjon(aksx, aksy);
			aksy *=-1;
			nedovery = true;
			
		}
		
		 if(nedoverx == true && xpos >= windows_width/2  ) {
			//fw.set_aksellerasjon(aksx, -aksy);
			aksx *= -1;
			nedoverx = false;
			
		}
       if(nedoverx == false && xpos <= windows_width/2  ) {
			//fw.set_aksellerasjon(aksx, aksy);
			aksx *= -1;
			nedoverx = true;
			
		}
		fw.set_aksellerasjon(aksx, aksy);
    }
   
    SDL_Quit();
}
