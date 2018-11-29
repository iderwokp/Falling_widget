#ifndef SDLSUTEX_WRAP_H
#define SDLSUTEX_WRAP_H
#include <iostream>
#include <string>
#include <SDL.h>


namespace Iderwok {
class  SDLSuTexWrap {
	
	   std::string filename{};
	   SDL_Renderer* renderer_{nullptr};
	   SDL_Surface* surface_{nullptr};
 	   SDL_Texture* texture_{nullptr};
 	   
	
 	   
 	   
 	   	void init() {
	    //std::cout << "init()\n";
//	      int imgFlags = IMG_INIT_PNG;
//	  	  IMG_Init( imgFlags ) & imgFlags;
//	  	  surface_ = IMG_Load(filename.c_str());
	  	  surface_ = SDL_LoadBMP(filename.c_str());
	  	  
//	  	  surface_->format->Amask = 0xff000000; 
//		  surface_->format->Ashift = 32; 
	  	  texture_ = SDL_CreateTextureFromSurface(renderer_, surface_);
		}
		
		void destroy() {
      	  SDL_DestroyTexture(texture_);
      	  SDL_FreeSurface(surface_);
		}
		
    public:
    	void change_sprite(const std::string& fn) {
    		filename = fn;
    			init();
    		//surface_ = SDL_LoadBMP(fn.c_str());
    	}
    	
    	SDLSuTexWrap() {/*std::cout << "SDLSuTexWrap()\n";*/}//= default;
    	SDLSuTexWrap(const std::string& fn, SDL_Renderer* r) : filename{fn}, renderer_{r} {
    		//std::cout << "SDLSuTexWrap(-----)\n";
			init();
		}
		SDLSuTexWrap(const SDLSuTexWrap& rhs) : filename{rhs.filename}, renderer_{rhs.renderer_} {
			init();
		}
		SDLSuTexWrap& operator=(const SDLSuTexWrap& rhs) {
		//	std::cout << "SDLSuTexWrap& operator=(&)\n";
			filename = rhs.filename;
			renderer_= rhs.renderer_;
			destroy(); //slette gamle texture og surface
			init(); //Lage ny texture og surface
			return *this;
		}
		SDLSuTexWrap(SDLSuTexWrap&& rhs) noexcept: filename{rhs.filename}, renderer_{rhs.renderer_}, surface_{rhs.surface_}, texture_{rhs.texture_}  {
			rhs.surface_ = nullptr;
			rhs.texture_ = nullptr;
			rhs.renderer_ = nullptr;
			rhs.filename = "";
		}
		SDLSuTexWrap& operator=(SDLSuTexWrap&& rhs) noexcept {
			//std::cout << "SDLSuTexWrap& operator=(&&)\n";
			destroy(); //slette gamle texture og surface
			filename = rhs.filename; 
			renderer_ = rhs.renderer_; 
			surface_ = rhs.surface_;
			texture_ = rhs.texture_;
			rhs.surface_ = nullptr;
			rhs.texture_ = nullptr;
			rhs.renderer_ = nullptr; 
			rhs.filename = "";
			return *this;
			
		}
		
		SDL_Surface* surface() const{ return surface_; }
		void set_surface(SDL_Surface* s) { surface_ = s; }
		
		SDL_Renderer* renderer() const{ return renderer_; }
		void set_renderer(SDL_Renderer* r) { renderer_ = r; }
		
		SDL_Texture* texture() const{ return texture_; }
		void set_texture(SDL_Texture* t) { texture_ = t; }
		
		
    	~SDLSuTexWrap() {
    	//	std::cout << "~SDLSuTexWrap()\n";
    		destroy();
    	}
	
	
};


}//namespace
#endif //SDLSUTEX_WRAP_H

