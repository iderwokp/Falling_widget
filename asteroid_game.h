#ifndef ASTEROID_GAME_H
#define ASTEROID_GAME_H

#include <SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <string>
#include "sdl_wrap.h"
#include "Falling_widget.h"
#include "vec2d.h"
#include "ammo.h"
#include "asteroids.h"



namespace Iderwok {

class Asteroid_game {
	
	public:
		Asteroid_game(const std::string& wn, int ww, int wh);
		~Asteroid_game() {
			SDL_Quit();
		}
		void start_game();
	private:
		Uint32 m_delay_time{2}; //Pause i ms mellom hver frame
		std::string m_window_name{};
		const std::string m_romskip_widget{"ball2.bmp"};
		const std::string m_asteroid_widget{"brick.bmp"};
		std::array<std::string, 2> m_sprites = {"ball2.bmp", "ball2_2.bmp"};
		int m_windows_width{};
		int m_windows_height{};
		bool m_quit{false};
		SDL_Renderer* m_renderer;
		Falling_widget m_romskip;
		std::vector<Asteroid> m_asteroids;
		std::vector<Ammo> m_ammo;
		double m_midwinX{}; //Midt i vinduet i X-retning
		double m_midwinY{}; //Midt i vinduet i Y-retning
		SDL_Event m_event;
		int m_rot_angle{0};
		bool m_changeSpr{false};
		bool m_normal_sprite{true}; //Hjelpe å styre valg mellom sprites
		double m_trust{0};
		bool m_shoot{false};
		const Sdl_wrap m_sdlwrap;
		const int m_romskipwidget_width{17};
    	const int m_romskipwidget_height{30};
    	const int m_asteroidwidget_width{50};
    	const int m_asteroidwidget_height{50};
    	const int m_asteroide_generasjon{2};
    	const int m_antall_asteroider{2};
    	
		void EventHandler();
		void init_SDL();
		void init_widgets();
		void enter_game_loop();
		void update_ammo( );
		void update_asteroids();
		void animate_romskip();
		void winner();//Midlertidig winner-funksjon
		void loose();//Midlertidig looser-funksjon
		template <typename T>
		void check_limits(T& romskip);
		
};


#endif

Asteroid_game::Asteroid_game(const std::string& wn, int ww, int wh): m_window_name{wn}, m_windows_width{ww}, m_windows_height{wh}, 
			m_midwinX{m_windows_width/2}, m_midwinY{m_windows_height/2}, m_sdlwrap{m_window_name, m_windows_width, m_windows_height} 
{
	init_SDL();
	
}

void Asteroid_game::init_SDL() {
	//m_sdlwrap = Sdl_wrap(m_windows_name, m_windows_width, m_windows_height);
	m_renderer = m_sdlwrap.renderer();
}

void Asteroid_game::init_widgets() {
	m_romskip = Falling_widget(m_romskip_widget, m_renderer, Point{m_midwinX, m_midwinY}, m_romskipwidget_width, m_romskipwidget_height, m_windows_height, m_windows_width, 0, 0, 0);
	
	for(int i = 0;i<m_antall_asteroider;++i) {
   		m_asteroids.emplace_back(m_asteroid_widget, m_renderer, 1.0, 0, 0, m_asteroidwidget_width, m_asteroidwidget_height, m_windows_height, m_windows_width, 0, m_asteroide_generasjon);
   	}
   	
}

void Asteroid_game::start_game() {
	m_romskip.setXY(m_midwinX-m_romskipwidget_width/2, m_midwinY-m_romskipwidget_height/2);
	enter_game_loop();
	
}

void Asteroid_game::EventHandler() {
	
    SDL_PollEvent(&m_event);
    if(m_event.type == SDL_QUIT){
        m_quit = true;
    }
	else if(m_event.type == SDL_KEYDOWN) {
	        if(m_event.key.keysym.sym == SDLK_RIGHT ) {
	            m_rot_angle+=5;
	            if(m_rot_angle > 360) m_rot_angle -=360;
	        }
	        if(m_event.key.keysym.sym == SDLK_LEFT ) {
	            m_rot_angle-=5;
	            if(m_rot_angle < 0) m_rot_angle +=360;
	        }
	        if(m_event.key.keysym.sym == SDLK_UP ) {
	            m_trust = 9.81/600;
	            m_changeSpr = true;
	        }
	        if(m_event.key.keysym.sym == SDLK_DOWN ) {
	            
	        }
	        if(m_event.key.keysym.sym == SDLK_a ) {
	        	if (!m_event.key.repeat) m_shoot = true;
	        	//std::cout << "event.key.repeat = " << event.key.repeat << "\n";
	        	
	        }
	}
    else if(m_event.type == SDL_KEYUP) {
        	if(m_event.key.keysym.sym == SDLK_UP ) {
           	 m_trust = 0.0;
           	 m_changeSpr = false;
       		 }
       		 if(m_event.key.keysym.sym == SDLK_DOWN ) {
           		 m_rot_angle += 180;
      	     }
      		  if(m_event.key.keysym.sym == SDLK_a ) {
	            m_shoot = false;
	            
	            
	          }
    }
      
}
void Asteroid_game::enter_game_loop() {
	
	while(!m_quit) {
	
        EventHandler();//, windows_width, windows_height);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer); 
        
        check_limits<Falling_widget>(m_romskip);
		romskip.updateXY();
	    
		bool pang = chrash_test(); //Kræsjer romskipet i en asteroide?
	    if(pang) {
			std::cout << "PANG!\n";
			loose();
			m_quit = true;
		}
	    
	    romskip.set_rot_angle(m_rot_angle);
	    Vec2d<double> aksvec(m_trust, (int)m_rot_angle-90);
	    Vec2d<double> tot_aks = aksvec;// + tyngdekraft;
	    m_romskip.set_aksellerasjon(tot_aks.xVal(), static_cast<float>(tot_aks.yVal()));
	    
	    if(m_changeSpr) {animate_romskip(); m_normal_sprite = false;}
	    else if (m_normal_sprite == false) {m_romskip.change_sprite("ball2.bmp"); m_normal_sprite = true;}
	    if(m_shoot) {
	    	
			m_ammo.emplace_back("ball.bmp", renderer, 90, Vec2d<double>{15.0,m_rot_angle-90}, Point{romskip.current_pos().X+fwidget_width/2, romskip.current_pos().Y+fwidget_height/2}, 
										5, 3, windows_height, windows_width,0);
			m_shoot = false;
		}
		update_asteroids();
		if(m_asteroids.size()== 0) {
			std::cout << "WIN!\n";		
			winner();
			quit = true;
		}
		
		update_ammo();
		if(m_ammo.size() != 0) check_hit();
		SDL_RenderPresent(m_renderer);
		
		SDL_Delay(m_delay_time); 
       
       
       
    }
}

void Asteroid_game::update_ammo( ) {
	auto ammo_it = m_ammo.begin();
    while(ammo_it != m_ammo.end()) {
    	ammo_it->updateXY();
    	
    	check_limits<Ammo>(*ammo_it);
    	ammo_it->dec_levetid();
    	if(ammo_it->levetid() == 0) ammo_it = m_ammo.erase(ammo_it);
    	else ++ammo_it;
    }
    	
}
void Asteroid_game::update_asteroids( ) {
	auto ast_it = m_asteroids.begin();
    while(ast_it != m_asteroids.end()) {
    	ast_it->updateXY();
    	check_limits<Asteroid>(*ast_it);

    	++ast_it;
    }
}

void Asteroid_game::animate_romskip() {
	static int counter{0};
	m_romskip.change_sprite(m_sprites[counter]);
	++counter;
	if (counter == 2) counter = 0; 
	
}

template <typename T>
void Asteroid_game::check_limits(T& romskip) { 
//En slags samleprosedyre
//Tar seg av hva som skjer når romskipet kommer utenfor kanten eller kjører veldig sakte
//Ikke helt stuerent å ha flere oppgaver i en funksjon men...
	//double loss{0.35}; //Evnen til å sprette
	int yPos = romskip.current_pos().Y;
    int xPos = romskip.current_pos().X;
    if (xPos < 0) 			   	{romskip.setXY(windows_width, yPos); }
    if (xPos > windows_width)	 {romskip.setXY(0, yPos); }
    if (yPos < 0) 				{romskip.setXY(xPos, windows_height);}
    if (yPos > windows_height)	 {romskip.setXY(xPos, 0);}
    if(fabs(romskip.velocityX()) < 0.005 && trust == 0.0) romskip.set_velocityX(0.0);
    if(fabs(romskip.velocityY()) < 0.005 && trust == 0.0) romskip.set_velocityY(0.0);
}
void Asteroid_game::winner() {
	Widget w ("winner.bmp", rend, {50, 50}, m_windows_width-300, m_windows_height-300);//, double dx = 0, int dy = 0, int a= 0 )
	w.moveTo(100,100);
	SDL_RenderPresent(m_renderer);
	SDL_Delay(2000);
}
void Asteroid_game::loose() {
	Widget w ("pang.bmp", rend, {100, 100}, m_windows_width-300, m_windows_height-300);//, double dx = 0, int dy = 0, int a= 0 )
	w.moveTo(100,100);
	SDL_RenderPresent(m_renderer);
	SDL_Delay(2000);
}



}//namespace












