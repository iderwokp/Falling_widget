#ifndef SDL_WRAP_H
#define SDL_WRAP_H
#include <string>
#include <iostream>

class  Sdl_wrap {
	private:
		std::string window_name{};
		int xPos{SDL_WINDOWPOS_UNDEFINED};
		int yPos{SDL_WINDOWPOS_UNDEFINED};
		int width{0};
		int height{0};
		Uint32 flags{0};
		SDL_Window* window_;
		SDL_Renderer* renderer_;
	
	public:
		Sdl_wrap(const std::string& wn, int w, int h): window_name{wn},  width{w},height{h} {
			std::cout << "Sdl_wrap()\n";
			window_ = SDL_CreateWindow(window_name.c_str(),xPos, yPos, width, height, flags);
			renderer_ = SDL_CreateRenderer(window_, -1, 0);
			
		}
		Sdl_wrap(const std::string& wn, int x, int y, int w, int h, Uint32 f): window_name{wn}, xPos{x}, yPos{y}, width{w},height{h},  flags{f} {
			
			window_ = SDL_CreateWindow(window_name.c_str(),xPos, yPos, width, height, flags);
			renderer_ = SDL_CreateRenderer(window_, -1, 0);
		
		}
		
		SDL_Window* window() const { return window_;}
		SDL_Renderer* renderer()const { return renderer_;}
		
		~Sdl_wrap() {
			std::cout << "~Sdl_wrap()\n";
			SDL_DestroyRenderer(renderer_);
			SDL_DestroyWindow(window_);
		}
	
};

#endif//SDL_WRAP_H

