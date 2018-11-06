#ifndef FALLING_WIDGET_H
#define FALLING_WIDGET_H


#include "widget.h"
#include "virt_coord_converter.h"
#include "aksellerasjon.h"
#include "velocity.h"

class Falling_widget: public Widget {
	public:
		Falling_widget(std::string fn, SDL_Renderer* rend, Point p={0, 0}, int w = 0, int h = 0, int ybound=0, int xbound=0, int scale=0, double vx0 = 0, double vy0 = 0, int rot= 0): 
							Widget(fn, rend, p, w, h, vx0, vy0, rot),
							yboundary_{ybound}, xboundary{xbound}, scale_{scale}, startpos_{p}  {
							
			velocity.vx0_= vx0;
			velocity.vy0_= vy0; 
							
		}
		
		
		void updateXY();
		
		//bool& boost() {return boost_;}
		//double& loss() { return loss_;}
		void set_aksellerasjon(float x, float y);
		void set_aksellerasjon(float h, int akse);
		
	private:
		void set_widget_xy();
		double next_Y();
		double next_X();
		
		static constexpr double g_ = 9.81;
		
		//double loss_{};
		Aksellerasjon aksellerasjon_{0.0f, 0.0f};
		int yboundary_{};
		int xboundary{};
		int scale_{};
		Velocity velocity{0, 0, 0, 0, 0};
	
		Point startpos_{};

		int tid_{1};
	
		//bool boost_{false};
		//bool nedover{true};
	
	
};
void Falling_widget::updateXY() {
			//std::cout << "xPos = " << xPos << "\n";
			set_widget_xy();
			//handle_side_crash(windows_width);
		
		}
void Falling_widget::set_aksellerasjon(float x, float y) {
	aksellerasjon_.X(x);
	aksellerasjon_.Y(y);
	
}
void Falling_widget::set_aksellerasjon(float h, int akse) {
	aksellerasjon_.angle(akse);
	aksellerasjon_.hyp(h);
	aksellerasjon_.calc_XY();
	
}
void Falling_widget::set_widget_xy() {
	//std::cout << "x = " << x << "\n";
	Vc_conv vc(Grav_heading::down, xboundary, yboundary_);
	
	//double y = next_distance();
	double y = next_Y();
	double x = next_X();
	++tid_;
	//std::cout << "x = " << x << "   y = " << y << "\n";
	auto [xx, yy] = vc.convert_from_virtual(x, y);
	//std::cout << "xx = " << xx << "   yy = " << yy << "\n\n";
	moveTo(xx,yy);
	
}

double Falling_widget::next_Y() {
	//std::cout << "next_Y(): velocity.vy0_ = " << velocity.vy0_ << "\n";
	velocity.va_ = aksellerasjon_.Y() * tid_;
	int s00 = startpos_.Y*scale_;
	double s = s00 + velocity.vy0_*tid_ + velocity.va_*tid_/2;
	velocity.vtot_ = velocity.vy0_ + velocity.va_;

	return s/scale_;
}
double Falling_widget::next_X() {
	//std::cout << "next_X(): velocity.vx0_ = " << velocity.vx0_ << "\n";
	velocity.va_ = aksellerasjon_.X() * tid_;
	int s00 = startpos_.X*scale_;
	double s = s00 + velocity.vx0_*tid_ + velocity.va_*tid_/2; 
	velocity.vtot_ = velocity.vx0_ + velocity.va_;

	return s/scale_;
}
#endif

