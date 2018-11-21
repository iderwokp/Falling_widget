#ifndef AMMO_H
#define AMMO_H

#include "Falling_widget.h"
#include "Vec2d.h"
namespace Iderwok {
class  Ammo: public Falling_widget {
	
	private:
		
		int m_levetid{};
		Vec2d<double> m_hastighet{};
		
	public:
		//Ammo() = default;
		Ammo(const std::string& fn, SDL_Renderer* rend, int lv, Vec2d<double> v, Point p={0, 0}, int widget_width = 0, int widget_height = 0, int ybound=0, int xbound=0, int rot= 0): 
								Falling_widget(fn, rend, p, widget_width, widget_height, ybound, xbound, v.xVal(), v.yVal(), rot),m_levetid{lv}, m_hastighet{v}
								
							  {
							  	
							  }
							//Falling_widget(const std::string& fn, SDL_Renderer* rend, Point p={0, 0}, int w = 0, int h = 0, int ybound=0, int xbound=0, double vx0 = 0, double vy0 = 0, int rot= 0)
		int levetid() const {return m_levetid;}
		void dec_levetid() {--m_levetid;}
};

}//namespace
#endif

