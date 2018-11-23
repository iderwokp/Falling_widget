#ifndef ASTEROID_H
#define ASTEROID_H
#include <random>
#include<chrono>
#include "Falling_widget.h"
#include "Vec2d.h"

namespace Iderwok {
	
class  Asteroid: public Falling_widget{
	private:
		int m_start_speed{};
		int m_generasjon{};
		int rand_position(int high_limit );
		//Vec2d<double> m_hastighet{10.0, (int)rand_position(360 )};
		Vec2d<double> m_hastighet{};
		 
	public:
		Asteroid(const std::string& fn, SDL_Renderer* rend, Vec2d<double> v, Point p={0, 0}, int widget_width = 0, int widget_height = 0, int ybound=0, int xbound=0, int rot= 0): 
								Falling_widget(fn, rend, p, widget_width, widget_height, ybound, xbound, v.xVal(), v.yVal(), rot),m_hastighet{v} {}		
		
		//Ctor med random plasseringer
		Asteroid(const std::string& fn, SDL_Renderer* rend, int ss, int widget_width = 0, int widget_height = 0, int ybound=0, int xbound=0, int rot= 0, int gen = 2): 
										 m_start_speed{ss}, m_generasjon{gen},
										Falling_widget(fn, rend, {static_cast<double>(rand_position(xbound-10 )), static_cast<double>(rand_position(ybound-10 ))}, widget_width, widget_height, ybound, xbound, rot) 
										
										{
											m_hastighet = set_random_velocity(m_start_speed);
											//std::cout << "m_hastighet.xVal() = " << m_hastighet.xVal() << "\tm_hastighet.yVal() = " << m_hastighet.yVal() << "\n";
										}							
															
							  
	int get_start_speed() { return m_start_speed;}
	void set_start_speed(int s) {m_start_speed = s;}
	Vec2d<double> get_hastighet() {return m_hastighet;}
	int get_generasjon() { return m_generasjon;}
	void set_generasjon(int g){ m_generasjon  = g;}
};

#endif

int Asteroid::rand_position(int high_limit )  {
	 	std::default_random_engine                  rand_dev;
	rand_dev.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  distr(0, high_limit);
	 return distr(generator);
 }


}//namespace

//#include <random>
//int rand_position( )
// {
// static std::default_random_engine e{};
// static std::uniform_int_distribution<int> d{1, 6};
// return d(e);
// }
