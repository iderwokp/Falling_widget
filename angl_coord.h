#ifndef ANGL_COORD_H
#define ANGL_COORD_H
#include <cmath>

namespace Iderwok {
constexpr float PI = 3.1415926;

class Angl_coord {
	public:
		Angl_coord(int ww, int wh): windows_width_{ww}, windows_height_{wh} {
			desentrX = windows_width_/2; //Midt i
			desentrY = windows_height_/2; //Midt i
		
		}
		std::pair<double, double> polXY(double x, double y) {
			std::pair<double, double> dd =  xy_to_pol(x, y);
			return 	pol_to_new_xy(dd);			
		}
		
	private:
		std::pair<double, double> xy_to_pol(double x, double y) {
			//std::cout << "xy_to_pol: windows_width_ = " << 360.0f/windows_width_ << "  x = " << x << "   y = " << y << "\n";
			return std::make_pair(x*(360.0f/windows_width_), windows_height_ - y);
		}
		std::pair<double, double> pol_to_new_xy(std::pair<double, double> xa) {
			double x = xa.second*cos(xa.first*(PI/180.0)); 
			double y = xa.second*sin(xa.first*(PI/180.0)); 
			
			//std::cout << "xy_to_pol: x = " << x << "   y = " << y << "\n";
			return std::make_pair(x/2+desentrX, y/2+desentrY); // y/2 og x/2 pga senter ligger midt i skjermen, og ikke i et hjørne
		}
		
		int windows_width_{0};
		int windows_height_{0};
		double desentrY{0};
		double desentrX{0};
	
	
};

}//namespace
#endif //ANGL_COORD_H

