#ifndef WIDGET_H
#define WIDGET_H
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "SDLSuTex_wrap.h"

namespace Iderwok {
struct Point {
    double X{0.0};
    double Y{0.0};
};

class Widget {
	// Private section
	std::string filename{};
	Point startpoint_{};
	Point current_pos_{};
	int width_{0};
	int height_{0};

	double deltaX_{0};
	int deltaY_{0};
	int angle_{0};
	bool rotating_{false};
	//Rect _rect;


	SDLSuTexWrap sutex;
    SDL_Rect rect_{0, 0, 0, 0};

	public:
	   // Widget() { std::cout << "Widget()\n" ; }
	    Widget(const std::string& fn, SDL_Renderer* rend, Point p={0, 0}, int w = 0, int h = 0, double dx = 0, int dy = 0, int a= 0 ): filename{fn},
                                                                                                                    //renderer{rend},
                                                                                                                    startpoint_{p},
                                                                                                                    width_{w},
                                                                                                                    height_{h},
                                                                                                                    deltaX_{dx},
                                                                                                                    deltaY_{dy},
                                                                                                                    angle_{a} {

	        sutex = SDLSuTexWrap(filename, rend);
	        current_pos_ = startpoint_;

	    }
		void change_sprite(const std::string& fn) {
			sutex.change_sprite(fn);
		}
	    SDL_Rect rect() const {
	        return SDL_Rect{static_cast<int>(startpoint_.X), static_cast<int>(startpoint_.Y), width_, height_};
	    }

	    void make(Point p, int w, int h) {
	        //std::cout << "make(Point, int, int)\n";
	        startpoint_ = p;
	        width_ = w;
	        height_ = h;
	        rect_ = {static_cast<int>(startpoint_.X), static_cast<int>(startpoint_.Y), width_, height_};
	    }
	    void make() {
	        //std::cout << "make()\n";
	        rect_ = {static_cast<int>(startpoint_.X), static_cast<int>(startpoint_.Y), width_, height_};
	    }
	    void show()const {
	        //std::cout << "show()\n";

            SDL_RenderCopyEx(sutex.renderer(), sutex.texture(), NULL, &rect_,angle_,nullptr,SDL_FLIP_NONE);
	    }

		int rot_angle()  const { //rotating angle
		    return angle_;
		}
		void set_rot_angle(int a) {
			angle_ = a;
		}
		bool is_rotating() const {
		    return rotating_;
		}
		void set_rotating(bool r) {
		    rotating_ = r;
		}
		Point current_pos() const {
			return current_pos_;
		}

		Point startpoint() const {
		    return startpoint_;
		}
		void startpoint(int x, int y)  {
		    startpoint_.X = x;
		    startpoint_.Y = y;
		}

		int width() const {
		    return width_;
		}
		void set_width(int w) { width_ = w;}

		int height() const {
		    return height_;
		}
		void set_height(int h) { height_ = h;}

		double deltaX() const {
		    return deltaX_;
		}
		int deltaY() const {
		    return deltaY_;
		}
		void set_deltaX(int x) {
		    deltaX_ = x;
		}
		void set_deltaY(int y) {
		    deltaY_ = y;
		}
		void moveTo(int x, int y) {
		    startpoint(x, y);
		    current_pos_ = startpoint_;
		    make();
		    show();
			//std::cout << "Widget::moveTo()  x = " << x << "\ty = " << y << "\n";
		}
		void moveTo(Point p) {
		    moveTo(p.X, p.Y);
		}

		friend bool operator==(const Widget& w1, const Widget& w2);
		friend bool operator!=(const Widget& w1, const Widget& w2);

};
bool operator==(const Widget& w1, const Widget& w2) {//TODO: Bør kanskje sjekke mere enn bare X,Y
    return (w1.startpoint_.X == w2.startpoint_.X && w1.startpoint_.Y == w2.startpoint_.Y) &&
	(w1.filename == w2.filename);
}
bool operator!=(const Widget& w1, const Widget& w2) {
    return !(w1 == w2);
}

//Helpers
void move_widget(Widget&);


bool widget_side_crash(Widget& w1, Widget& w2) {
    if(w1.startpoint().X + w1.width() <= w2.startpoint().X || w1.startpoint().X  >= w2.startpoint().X +w2.width() ) return false;
    if(w1.startpoint().Y + w1.height() <= w2.startpoint().Y) return false;
    if(w1.startpoint().Y >= w2.startpoint().Y + w2.height()) return false;
    return true;
}
bool widget_top_bottom_crash(Widget& w1, Widget& w2) {
    if(w1.startpoint().Y + w1.height() <= w2.startpoint().Y || w1.startpoint().Y  >= w2.startpoint().Y + w2.height()) return false;
    if(w1.startpoint().X + w1.width() <= w2.startpoint().X) return false;
    if(w1.startpoint().X >= w2.startpoint().X+w2.width()) return false;
    return true;

}
void handle_widget_crash(Widget& w1, Widget& w2) {
     if(widget_side_crash(w1, w2) || widget_side_crash(w2, w1)) {
        //std::cout << "Pang side \n Før setdeltaX_ w1.deltaX() == " << w1.deltaX() << "\n ";
        w1.set_deltaX(-w1.deltaX());
        w2.set_deltaX(-w2.deltaX());
        w1.set_rotating(true);// == true;
        w2.set_rotating(true);// == true;
        move_widget(w1);
        move_widget(w2);

    }
    if(widget_top_bottom_crash(w1, w2) || widget_top_bottom_crash(w2, w1)) {
        //std::cout << "Pang top/bottom \n Før set_DeltaY w1.deltaY() == " << w1.deltaY() << "\n ";
        w1.set_deltaY(-w1.deltaY());
        w2.set_deltaY(-w2.deltaY());
        w1.set_rotating(true);// == true;
        w2.set_rotating(true);// == true;
        move_widget(w1);
        move_widget(w2);

    }
}


void move_widget(Widget& w) {

        int dx = w.deltaX();
        int dy = w.deltaY();
        w.moveTo(w.startpoint().X + dx, w.startpoint().Y + dy);
}
void handle_top_bottom_crash(Widget& w, int windowsheight_) {
    if(w.startpoint().Y < 0 || w.startpoint().Y > (windowsheight_ - w.height())) {
         //w.deltaY() = -w.deltaY();
         w.set_deltaY(-w.deltaY());
    }
}
void handle_side_crash(Widget& w, int windowswidth_) {
    if(w.startpoint().X < 0 || w.startpoint().X > (windowswidth_ - w.width())) {
        //w.deltaX() = -w.deltaX();
        w.set_deltaX(-w.deltaX());
    }
}

}//namespace
#endif


