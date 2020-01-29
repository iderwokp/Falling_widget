#ifndef ASTEROID_GAME_H
#define ASTEROID_GAME_H

#include <SDL2/SDL.h>
#include "sdl_wrap.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <string>
#include <algorithm>
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
		const Uint32 m_delay_time{2}; //Pause i ms mellom hver frame
		std::string m_window_name{};
		const std::string m_bakgrunn_widget{"bakgrunn.bmp"};
		const std::string m_romskip_widget{"ball2.bmp"};
		const std::string m_asteroid_widget{"brick.bmp"};
		std::array<std::string, 2> m_sprites = {"ball2.bmp", "ball2_2.bmp"};
		int m_windows_width{};
		int m_windows_height{};
		bool m_quit{false};
		SDL_Renderer* m_renderer;
		Falling_widget m_romskip;
		Widget m_bakgrunn;
		std::vector<Asteroid> m_asteroids;
		std::vector<Ammo> m_ammo;
		double m_midwinX{}; //Midt i vinduet i X-retning
		double m_midwinY{}; //Midt i vinduet i Y-retning
		SDL_Event m_event;
		Vec2d<double> m_tyngdekraft{0.0, 0.0};//9.81/800};//Ekstra tyngdekraft som skal påvirke
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
		void winner() const;//Midlertidig winner-funksjon
		void loose() const;//Midlertidig looser-funksjon
		template <typename T>
		void check_limits(T& romskip);
		template <typename T>
		bool chrash_test(T& as, Point p);
		void check_hit();
		template <typename T>
		bool hit(T& a, int mx, int my);
		template <typename T>
		bool hit(T& a, Point p);

};


#endif

Asteroid_game::Asteroid_game(const std::string& wn, int ww, int wh): m_window_name{wn}, m_windows_width{ww}, m_windows_height{wh},
			m_midwinX{m_windows_width/2}, m_midwinY{m_windows_height/2}, m_sdlwrap{m_window_name, m_windows_width, m_windows_height}
{
	init_SDL();
	init_widgets();
}

void Asteroid_game::init_SDL() {
	//m_sdlwrap = Sdl_wrap(m_windows_name, m_windows_width, m_windows_height);
	m_renderer = m_sdlwrap.renderer();

}

void Asteroid_game::init_widgets() {
	m_romskip = Falling_widget(m_romskip_widget, m_renderer, Point{m_midwinX, m_midwinY}, m_romskipwidget_width, m_romskipwidget_height, m_windows_height, m_windows_width, 0, 0, 0);
	m_bakgrunn = Widget(m_bakgrunn_widget, m_renderer,Point{0,0}, m_windows_width, m_windows_height, 0, 0, 0);
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
	            m_rot_angle+=1;
	            if(m_rot_angle > 360) m_rot_angle -=360;
	        }
	        if(m_event.key.keysym.sym == SDLK_LEFT ) {
	            m_rot_angle-=1;
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
        SDL_RenderClear(m_renderer);
        m_bakgrunn.moveTo(0,0);
        check_limits<Falling_widget>(m_romskip);
		m_romskip.updateXY();

		bool pang = chrash_test(m_asteroids, m_romskip.current_pos()); //Kræsjer romskipet i en asteroide?
	    if(pang) {
			std::cout << "PANG!\n";
			loose();
			m_quit = true;
		}

	    m_romskip.set_rot_angle(m_rot_angle);
	    Vec2d<double> aksvec(m_trust, (int)m_rot_angle-90);
	    Vec2d<double> tot_aks = aksvec + m_tyngdekraft;
	    m_romskip.set_aksellerasjon(tot_aks.xVal(), static_cast<float>(tot_aks.yVal()));

	    if(m_changeSpr) {animate_romskip(); m_normal_sprite = false;}
	    else if (m_normal_sprite == false) {m_romskip.change_sprite("ball2.bmp"); m_normal_sprite = true;}
	    if(m_shoot) {

			m_ammo.emplace_back("ball.bmp", m_renderer, 90, Vec2d<double>{15.0,m_rot_angle-90}, Point{m_romskip.current_pos().X+m_romskipwidget_width/2, m_romskip.current_pos().Y+m_romskipwidget_height/2},
										5, 3, m_windows_height, m_windows_width,0);
			m_shoot = false;
		}
		update_asteroids();
		if(m_asteroids.size()== 0) {
			std::cout << "WIN!\n";
			winner();
			m_quit = true;
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
    if (xPos < 0) 			   	{romskip.setXY(m_windows_width, yPos); }
    if (xPos > m_windows_width)	 {romskip.setXY(0, yPos); }
    if (yPos < 0) 				{romskip.setXY(xPos, m_windows_height);}
    if (yPos > m_windows_height)	 {romskip.setXY(xPos, 0);}
    if(fabs(romskip.velocityX()) < 0.007 && m_trust == 0.0) romskip.set_velocityX(0.0);
    if(fabs(romskip.velocityY()) < 0.007 && m_trust == 0.0) romskip.set_velocityY(0.0);
}
void Asteroid_game::winner() const {
	Widget w ("winner.bmp", m_renderer, {50, 50}, m_windows_width-300, m_windows_height-300);//, double dx = 0, int dy = 0, int a= 0 )
	w.moveTo(100,100);
	SDL_RenderPresent(m_renderer);
	SDL_Delay(2000);
}
void Asteroid_game::loose() const {
	Widget w ("pang.bmp", m_renderer, {100, 100}, m_windows_width-300, m_windows_height-300);//, double dx = 0, int dy = 0, int a= 0 )
	w.moveTo(100,100);
	SDL_RenderPresent(m_renderer);
	SDL_Delay(2000);
}

template <typename T>
bool Asteroid_game::chrash_test(T& as, Point p) {
	std::vector<Point> points;
	points.push_back({p.X+m_asteroidwidget_width/2, p.Y}); //spissen
	points.push_back({p.X, p.Y+m_asteroidwidget_height});//venstre hjørne
	points.push_back({p.X+m_asteroidwidget_width, p.Y+m_asteroidwidget_height});//høyre hjørne

	for(auto& a: as) {
		return std::any_of(begin(points), end(points),[&a, this](Point p) {return hit(a, p);});
	//	if(std::any_of(begin(points), end(points),[&a, this](Point p) {return hit(a, p);})) return true;
//		for(auto po: points) {//Kan bruke std::any_of her?
//			if(hit(a, po.X, po.Y)) return true;
//		}
	}
	return false; //For å slippe Warning: 277	[Warning] control reaches end of non-void function [-Wreturn-type]
}

template <typename T>
bool Asteroid_game::hit(T& a, int mx, int my) { // sjekker om en koordinat ligger inom en widget
	int yPos = a.current_pos().Y;
  	    int xPos = a.current_pos().X;
         if(mx > xPos+a.width()) {return false;}
         if(mx < xPos) {return false;}
         if(my > yPos+a.height()) {return false;}
         if(my < yPos) {return false;}
    return true;

}
template <typename T>
bool Asteroid_game::hit(T& a, Point p) { // sjekker om en koordinat ligger inom en widget
	return hit(a, p.X, p.Y);
}
void Asteroid_game::check_hit( ) {


	for(auto & am: m_ammo) {
		auto ast_it = m_asteroids.begin();
	    while(ast_it != m_asteroids.end()) {
	    	if(hit(*ast_it, am.current_pos().X, am.current_pos().Y)) {
	    		int xpos = ast_it->current_pos().X;
				int ypos = ast_it->current_pos().Y;
				//Vec2d<double> v = ast_it->get_hastighet();
				int start_fart = ast_it->get_start_speed()*1;
				ast_it = m_asteroids.erase(ast_it);
				if (ast_it->get_generasjon()>0) {
					int gen  = ast_it->get_generasjon(); gen--;
					int fw = ast_it->width()/2;
					int fh = ast_it->height()/2;
					m_asteroids.emplace_back("brick.bmp", m_renderer, start_fart, xpos+30, ypos-30,fw ,fh , m_windows_height, m_windows_width, 0, gen);
					m_asteroids.emplace_back("brick.bmp", m_renderer, start_fart, xpos-30, ypos+30, fw, fh, m_windows_height, m_windows_width, 0, gen);
					return;
				}
				//std::cout << "HIT " << "\n";

				}
	    	else ++ast_it;
	    }
	}
}

}//namespace












