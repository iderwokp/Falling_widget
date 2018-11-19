#ifndef VEC2D_H
#define VEC2D_H
#include <cmath>

namespace Iderwok{

const double m_PI{3.1415926535};
 template <typename T1> 
class Vec2d {
		T1 x_{};
		T1 y_{};
		T1 magn_{};
		int angle_{};
		static constexpr double PI{3.1415926535};
		void calc_XY();
		void calc_hyp_a();
	public:
		Vec2d() = default;
		Vec2d(T1 x, T1 y): x_{x}, y_{y} {calc_hyp_a();}
		Vec2d(T1 m, int angl): magn_{m}, angle_{angl}{calc_XY();}
		Vec2d& operator+=(const Vec2d& other);	
		Vec2d& operator-=(const Vec2d& other);	
		
		T1 length() const;
		T1 xVal() const { return x_;}
		T1 yVal() const { return y_;}
		void set_xVal(T1 val) { xVal = val;}
		void set_yVal(T1 val) { yVal = val;}
		
		//friend T1 dot_product(Vec2d v1, Vec2d v2);

};
template <typename T1> 
void Vec2d<T1>::calc_hyp_a(){
	magn_ = sqrt(x_*x_ + y_*y_);
	double radangl = acos(y_/magn_);
	angle_ = static_cast<int>(radangl/(PI/180.0));
}
template <typename T1> 
void Vec2d<T1>::calc_XY(){
	double rad = (angle_)*(PI/180.0);
	x_ = angle_*cos(rad);
	y_ = angle_*sin(rad);
	
	}
template <typename T1> 
T1 Vec2d<T1>::length() const {
	return sqrt(x_*x_ + y_*y_);	
}

template <typename T1> 
T1 operator*(const Vec2d<T1> v1, const Vec2d<T1> v2) { //Dot product
	return(v1.xVal()*v2.xVal() + v1.yVal()*v2.yVal());
}
template <typename T1> 
Vec2d<T1>& Vec2d<T1>::operator+=(const Vec2d<T1>& other) {
	this->x_ += other.x_;
	this->y_ += other.y_;
	return *this;
}
template <typename T1>
 Vec2d<T1> operator+(const Vec2d<T1> v1, const Vec2d<T1> v2) {
 	Vec2d v{v1};
 	v += v2;
 	return v;
 }
 template <typename T1>
Vec2d<T1>& Vec2d<T1>::operator-=(const Vec2d<T1>& other) {
	this->x_ -= other.x_;
	this->y_ -= other.y_;
	return *this;
}
template <typename T1>
 Vec2d<T1> operator-(const Vec2d<T1> v1, const Vec2d<T1> v2) {
 	Vec2d v{v1};
 	v -= v2;
 	return v;
 }
 template <typename T1> 
 T1 angle_rad(const Vec2d<T1>& v1, const Vec2d<T1>& v2) {
 		
 	return acos(v1*v2/(v1.length()*v2.length()));
 }
 template <typename T1> 
 T1 angle_deg(const Vec2d<T1>& v1, const Vec2d<T1>& v2) {
 		return angle_rad(v1, v2)*(180.0/m_PI);
 }
 
}//namespace

#endif

