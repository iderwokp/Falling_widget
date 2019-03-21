
namespace Iderwok {
struct Velocity {
	double vx0_{0}; //Startfart
	double vy0_{0}; //Startfart
	double vXtot_{0};
	double vYtot_{0};
	double vTot() {
		return sqrt(vXtot_*vXtot_ + vYtot_*vYtot_);	
	}
	double vmax{0};	
};

}//namespace
