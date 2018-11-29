#ifndef ASTEROID_H
#define ASTEROID_H
#include <random>
#include<chrono>
#include "Falling_widget.h"
#include "Vec2d.h"

namespace Iderwok {
	
class  Asteroid: public Falling_widget{
	private:
		double m_start_speed{};
		int m_generasjon{};
		//int rand_position(int high_limit);
		int posisjon(int h, int alternative );
		//Vec2d<double> m_hastighet{10.0, (int)rand_position(360 )};
		Vec2d<double> m_hastighet{};
		 
	public:
		Asteroid(const std::string& fn, SDL_Renderer* rend, const Vec2d<double>& v, Point p={0, 0}, int widget_width = 0, int widget_height = 0, int ybound=0, int xbound=0, int rot= 0): 
								Falling_widget(fn, rend, p, widget_width, widget_height, ybound, xbound, v.xVal(), v.yVal(), rot),m_hastighet{v} {}		
		
		//Ctor med random plasseringer
		Asteroid(const std::string& fn, SDL_Renderer* rend, double ss, int xpos=0, int ypos = 0,int widget_width = 0, int widget_height = 0, int ybound=0, int xbound=0, int rot= 0, int gen = 2): 
										 
										Falling_widget(fn, rend, {static_cast<double>(posisjon(xbound-10, widget_width )), static_cast<double>(posisjon(ybound-10, ypos ))}, widget_width, widget_height, ybound, xbound,0.0,0.0, rot) 
										,m_start_speed{ss}, m_generasjon{gen}
										{
											m_hastighet = set_random_velocity(m_start_speed);
											//std::cout << "m_hastighet.xVal() = " << m_hastighet.xVal() << "\tm_hastighet.yVal() = " << m_hastighet.yVal() << "\n";
										}							
															
							  
	int get_start_speed() { return m_start_speed;}
	void set_start_speed(int s) {m_start_speed = s;}
	//Vec2d<double> get_hastighet() {return m_hastighet;}
	int get_generasjon() { return m_generasjon;}
	void set_generasjon(int g){ m_generasjon  = g;}
};

#endif
int Asteroid::posisjon(int h, int a) {
	if (a == 0) return rand_position(h);
	else return a;
}

}//namespace


