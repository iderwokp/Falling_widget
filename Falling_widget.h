#ifndef FALLING_WIDGET_H
#define FALLING_WIDGET_H


#include "widget.h"
#include "virt_coord_converter.h"
#include "aksellerasjon.h"
#include "velocity.h"

namespace Iderwok {

class Falling_widget: public Widget {
	public:
		Falling_widget(const std::string& fn, SDL_Renderer* rend, Point p={0, 0}, int w = 0, int h = 0, int ybound=0, int xbound=0, double vx0 = 0, double vy0 = 0, int rot= 0): 
							Widget(fn, rend, p, w, h, vx0, vy0, rot),
							yboundary_{ybound}, xboundary_{xbound}, startpos_{p}  {
							
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
		
		Aksellerasjon aksellerasjon_{0.0f, 0.0f};
		int yboundary_{};
		int xboundary_{};
		Velocity velocity{0, 0, 0, 0, 0};
	
		Point startpos_{};
		Point currpos_{0.0f, 0.0f};
		const float drag_{0.9999f} ; //Motstand/friksjon. 1.0 er friksjonsfritt 0.0 er 100% friksjon
//		double Ypos_{0};
//		double Xpos_{0};
		//int tid_{1};
	
		
		
	
	
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
	Vc_conv vc(Grav_heading::down, xboundary_, yboundary_);
	
	double y = next_Y();
	double x = next_X();
//	++tid_;
	auto [xx, yy] = vc.convert_from_virtual(x, y);
	moveTo(xx,yy);
	
}

double Falling_widget::next_Y() {
	velocity.vYtot_ += velocity.vy0_;
	velocity.vy0_ = 0;
	velocity.vYtot_ += aksellerasjon_.Y();
	velocity.vYtot_*=drag_;
	double deltaS = velocity.vYtot_/2;
	//double s = velocity.vy0_*tid_ ;
	//currpos.Y += s;
	currpos_.Y += deltaS; 
	return (startpos_.Y+currpos_.Y);
}
double Falling_widget::next_X() {
	velocity.vXtot_ += velocity.vx0_;
	velocity.vx0_ = 0;
	velocity.vXtot_ += aksellerasjon_.X();	
	velocity.vXtot_*=drag_;											        
	double deltaS = velocity.vXtot_/2;
	//double s = velocity.vx0_*tid_ ;
	//currpos.X += s;
	currpos_.X += deltaS; 
	return (startpos_.X+currpos_.X);
}




}//namespace

#endif

//	velocity.va_ = aksellerasjon_.Y() * tid_;
//	velocity.vtot_ = velocity.vy0_ + velocity.va_;
//	velocity.vYtot_ = vtot

	//std::cout << "next_X(): velocity.vx0_ = " << velocity.vx0_ << "\n";
//	velocity.va_ = aksellerasjon_.X() * tid_;
//	velocity.vtot_ = velocity.vx0_ + velocity.va_;  
//	const int s00 = startpos_.X*scale_;
//	double s = s00 + velocity.vx0_*tid_ + velocity.va_*tid_/2; 

