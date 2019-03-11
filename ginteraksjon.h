#ifndef GINTERAKSJON_H
#define GINTERAKSJON_H

#include <vector>

#include "Falling_widget.h"
#include "vec2d.h"

namespace Iderwok {
class  Gravity_interaksjon{
	
	public:
		Gravity_interaksjon(const std::vector<Falling_widget> fw) : fwidget{fw} {}
		void start() { 
			
		}
	private:
		std::vector<Falling_widget> fwidget_{};
};

double Gravity_interaksjon::sanitize_angle(Point gjeldende, Point sjekke_mot, double angl) {
	if(gjeldende.X == (gjeldende.X+sjekke_mot.X) && gjeldende.Y > (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 0 << std::endl;*/return angl+180;}
	//if(gjeldende.X < sjekke_mot.X && gjeldende.Y > sjekke_mot.Y) return angl+270;
	if(gjeldende.X > (gjeldende.X+sjekke_mot.X) && gjeldende.Y > (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 1 << std::endl; */return 180+(180-angl);}//+2*(180-angl);
	if(gjeldende.X > (gjeldende.X+sjekke_mot.X) && gjeldende.Y < (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 2 << std::endl; */return angl;}
	if(gjeldende.X < (gjeldende.X+sjekke_mot.X) && gjeldende.Y > (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 3 << std::endl;*/ return 360-angl;}
	if(gjeldende.X < (gjeldende.X+sjekke_mot.X) && gjeldende.Y < (gjeldende.Y+sjekke_mot.Y)) {/*std::cout << 4 << std::endl; */return angl;}
	return angl;

}

} //namespace
#endif

