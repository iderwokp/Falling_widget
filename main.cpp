#include <SDL.h>


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
    
    
    while(index >=0 && !quit) {
	
        EventHandler(event, quit, windows_width, windows_height);
        
        
        
    }
    
    SDL_Quit();
}
