#ifndef VIRT_COORD_CONVERTER_H
#define VIRT_COORD_CONVERTER_H



#include <utility>  //std::pair()
#include "angl_coord.h"

namespace Iderwok {
enum class Grav_heading {down, up, right, left, point};

class Vc_conv { 
	public:
		Vc_conv() = default;
		Vc_conv(Grav_heading gv, int w, int h): gravh{gv}, windows_width_{w}, windows_height_{h} {}
		
		std::pair<double, double> convert_from_virtual(double, double);
	private:
		Grav_heading gravh {Grav_heading::down};
		int windows_width_{0};
		int windows_height_{0};
		
		std::pair<double, double> grav_down(double, double);
		std::pair<double, double> grav_up(double, double);
		std::pair<double, double> grav_right(double, double);
		std::pair<double, double> grav_left(double, double);
		std::pair<double, double> grav_point(double, double);
};

std::pair<double, double> Vc_conv::convert_from_virtual(double x, double y) {
	switch(gravh) {
		case Grav_heading::down:
			return grav_down(x, y);
			break;
		case Grav_heading::up:
			return grav_up(x, y);
			break;
		case Grav_heading::right:
			return grav_right(x, y);
			break;
		case Grav_heading::left:
			return grav_left(x, y);
			break;
		case Grav_heading::point:
			return grav_point(x, y);
		default:
			return grav_down(x, y);
	}
	
}
std::pair<double, double> Vc_conv::grav_down(double x, double y) {
	return std::make_pair(x, y);
}

std::pair<double, double> Vc_conv::grav_up(double x, double y) {
	return std::make_pair(x, windows_height_-y);
}

std::pair<double, double> Vc_conv::grav_right(double x, double y) {
	return std::make_pair(y, x); //TODO: er vel ikke helt rett
}

std::pair<double, double> Vc_conv::grav_left(double x, double y) {
	return std::make_pair(x, y); //Dummy. Denne funskjonen er ikke implementert. Ikke bruk denne
}
std::pair<double, double> Vc_conv::grav_point(double x, double y) {
	Angl_coord ac{windows_width_, windows_height_};
	
	return ac.polXY(x, y);
}
}//namespace
#endif//VIRT_COORD_CONVERTER_H



