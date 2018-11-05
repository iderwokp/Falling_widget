#ifndef FALLING_WIDGET_H
#define FALLING_WIDGET_H


#include "widget.h"
#include "virt_coord_converter.h"
#include "aksellerasjon.h"
#include "velocity.h"

class Falling_widget: public Widget {
	public:
		Falling_widget(std::string fn, SDL_Renderer* rend, Point p={0, 0}, int w = 0, int h = 0, int bound=0, int wbound=0, int scale=0, double vx0 = 0, double vy0 = 0, double sx0 = 0, double sy0 = 0, int rot= 0):
							Widget(fn, rend, p, w, h, vx0, vy0, rot),
							boundary_{bound}, wall_boundary{wbound}, scale_{scale}, sx0_{sx0}, sy0_{sy0} {
							
			velocity.vy0_= vy0;
			velocity.vy0_= vy0; 
							
		}
		
		//double next_distance(); //I en loop, vil denne gi neste Y-koordinat
		void updateXY(int windows_width);
		double next_step();
		void set_widget_xy();
		bool& boost() {return boost_;}
		double& loss() { return loss_;}
		void set_starthd(double y) { sy0_ = y;}
		void set_aksellerasjon(float, float);
		void set_aksellerasjon(float h, int a);
		
	private:
		
		double next_Y();
		double next_X();
		
		static constexpr double g_ = 9.81;
		
		double loss_{};
		Aksellerasjon aksellerasjon_{0.0f, 0.0f};
		int boundary_{};
		int wall_boundary{};
		int scale_{};
		Velocity velocity{0, 0, 0, 0, 0};
	
		double sx0_{0};
		double sy0_{0};
		int tid_{1};
	
		bool boost_{false};
		double startY{0};
		//bool nedover{true};
	
	
};
void Falling_widget::updateXY(int windows_width) {
			//std::cout << "xPos = " << xPos << "\n";
			//setXY(xPos);
			//setXY();
			set_widget_xy();
			//handle_side_crash(windows_width);
			//xPos += widget->deltaX();						
		}
void Falling_widget::set_aksellerasjon(float x, float y) {
	aksellerasjon_.X(x);
	aksellerasjon_.Y(y);
	
}
void Falling_widget::set_aksellerasjon(float h, int a) {
	aksellerasjon_.angle(a);
	aksellerasjon_.hyp(h);
	aksellerasjon_.calc_XY();
	
}
void Falling_widget::set_widget_xy() {
	//std::cout << "x = " << x << "\n";
	Vc_conv vc(Grav_heading::down, wall_boundary, boundary_);
	
	//double y = next_distance();
	double y = next_Y();
	double x = next_X();
	++tid_;
	//std::cout << "x = " << x << "   y = " << y << "\n";
	auto [xx, yy] = vc.convert_from_virtual(x, y);
	//std::cout << "xx = " << xx << "   yy = " << yy << "\n\n";
	moveTo(xx,yy);
	
}

//double FFGenerator::next_distance() {
//	if(nedover) return retning_nedover();
//	else return retning_oppover();
//	
//}
double Falling_widget::next_Y() {
	//std::cout << "next_Y(): velocity.vy0_ = " << velocity.vy0_ << "\n";
	velocity.va_ = aksellerasjon_.Y() * tid_;
	int s00 = sy0_*scale_;
	double s = s00 + velocity.vy0_*tid_ + velocity.va_*tid_/2;
	velocity.vtot_ = velocity.vy0_ + velocity.va_;
	
	if (s/scale_ > boundary_-25) { 
		std::cout << __func__ << "(): s = " << s << "  s/scale_ = " << s/scale_ << " velocity.vy0_ = " << velocity.vy0_ << "  velocity.vtot_ = " << velocity.vtot_ << "\n"; 	
		s = (boundary_-25)*scale_;
		//s = 13000;
		velocity.vy0_ = -velocity.vtot_;
		std::cout << __func__ << "() slutt: s = " << s << "  s/scale_ = " << s/scale_ << " velocity.vy0_ = " << velocity.vy0_ << "  velocity.vtot_ = " << velocity.vtot_ << "\n\n"; 	
	}
	return s/scale_;
}
double Falling_widget::next_X() {
	//std::cout << "next_X(): velocity.vx0_ = " << velocity.vx0_ << "\n";
	velocity.va_ = aksellerasjon_.X() * tid_;
	int s00 = sx0_*scale_;
	double s = s00 + velocity.vx0_*tid_ + velocity.va_*tid_/2; 
	velocity.vtot_ = velocity.vx0_ + velocity.va_;
	
	if (s/scale_ > boundary_-25) {
		std::cout  << __func__ << "(): s = " << s << "  s/scale_ = " << s/scale_ << " boundary_-25 = " << boundary_-25 << "\n"; 
		s = (boundary_-25)*scale_;
		velocity.vx0_ = -velocity.vtot_;
	}
	return s/scale_;
}
#endif

