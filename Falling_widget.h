#ifndef FALLING_WIDGET_H
#define FALLING_WIDGET_H

#include <random>
#include <chrono>
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
		void setXY(double x, double y);
		double velocityX() {return velocity.vXtot_;}
		double velocityY() {return velocity.vYtot_;}
		void set_velocityX(double v) {velocity.vXtot_ = v;}
		void set_velocityY(double v) {velocity.vYtot_ = v;}
		Vec2d<double> set_random_velocity(int m = 1);
	
	protected:
			int rand_position(int high_limit );
	private:
		
		void set_widget_xy();
		double next_Y();
		double next_X();
		
		Aksellerasjon aksellerasjon_{0.0f, 0.0f};
		int yboundary_{};
		int xboundary_{};
		Velocity velocity{0, 0, 0, 0, 0};
	
		Point startpos_{};
		Point currpos_{0.0, 0.0};
//		double Ypos_{0};
//		double Xpos_{0};
		//int tid_{1};
	
		
		
	
	
};
Vec2d<double> Falling_widget::set_random_velocity(int m) {
	int ang = rand_position(359 );
//	int magn = rand_position( m );
	//std::cout << "m = " << m << "    ang = " << ang << "\tmagn = " << magn << "\n";
	Vec2d<double> vec(m,ang);
	velocity.vx0_ = vec.xVal();
	velocity.vy0_ = vec.yVal();
	//std::cout << "velocity.vx0 = " << velocity.vx0_ << "velocity.vy0 = " << velocity.vy0_ << "\n";
	return vec;
}

int Falling_widget::rand_position(int high_limit )  {
	std::default_random_engine                  rand_dev;
	rand_dev.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  distr(0, high_limit);
	 return distr(generator);
 }


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
void Falling_widget::setXY(double x, double y) {
	Vc_conv vc(Grav_heading::down, xboundary_, yboundary_);
	auto [xx, yy] = vc.convert_from_virtual(x, y);
	moveTo(xx,yy);
	
	//Siden x,y koordinatene er oppdatert uten op next_X() og next_Y()
	//må det gjøres manuelt her
	currpos_.X = xx-startpos_.X;
	currpos_.Y = yy-startpos_.Y;
}
double Falling_widget::next_Y() {
	velocity.vYtot_ += velocity.vy0_;
	velocity.vy0_ = 0;
	velocity.vYtot_ += aksellerasjon_.Y();
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

