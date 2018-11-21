#ifndef ASTEROID_H
#define ASTEROID_H
#include <random>
#include "Falling_widget.h"
#include "Vec2d.h"

namespace Iderwok {
	
class  Asteroid: public Falling_widget{
	private:
		int rand_position(int high_limit )
		 {
			 static std::default_random_engine e{};
			 static std::uniform_int_distribution<int> d{10, high_limit};
			 return d(e);
		 }
	public:
		Asteroid(const std::string& fn, SDL_Renderer* rend, Vec2d<double> v, Point p={0, 0}, int widget_width = 0, int widget_height = 0, int ybound=0, int xbound=0, int rot= 0): 
								Falling_widget(fn, rend, p, widget_width, widget_height, ybound, xbound, v.xVal(), v.yVal(), rot),m_hastighet{v} {}		
		
		//Ctor med random plasseringer
		Asteroid(const std::string& fn, SDL_Renderer* rend, Vec2d<double> v, int widget_width = 0, int widget_height = 0, int ybound=0, int xbound=0, int rot= 0): 
										m_hastighet{v} {
									Point p = {rand_position(xbound-10 ), rand_position(ybound-10 )};
									Falling_widget(fn, rend, p, widget_width, widget_height, ybound, xbound, v.xVal(), v.yVal(), rot);
								}							
							  
	
};
}//namespace
#endif

#include <random>
int rand_position( )
 {
 static std::default_random_engine e{};
 static std::uniform_int_distribution<int> d{1, 6};
 return d(e);
 }
