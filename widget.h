#ifndef WIDGET_H
#define WIDGET_H
#include <SDL.h>
#include <iostream>
#include <string>
#include "SDLSuTex_wrap.h"


struct Point {
    double X{0.0};
    double Y{0.0};
};

class Widget {
	// Private section
	std::string filename{};
	Point _startpoint{};
	Point current_pos_{};
	int _width{0};
	int _height{0};
	
	double _deltaX{0};
	int _deltaY{0};
	int _angle{0};
	bool _rotating{false};
	//Rect _rect;
	

	SDLSuTexWrap sutex;
    SDL_Rect _rect{0, 0, 0, 0};

	public:
	   // Widget() { std::cout << "Widget()\n" ; }
	    Widget(const std::string& fn, SDL_Renderer* rend, Point p={0, 0}, int w = 0, int h = 0, double dx = 0, int dy = 0, int a= 0 ): filename{fn}, 
                                                                                                                    //renderer{rend},
                                                                                                                    _startpoint{p},                                                                                                                    
                                                                                                                    _width{w},
                                                                                                                    _height{h},
                                                                                                                    _deltaX{dx},
                                                                                                                    _deltaY{dy},
                                                                                                                    _angle{a} {
	        //std::cout << "Widget(................)\n";
		//std::cout << "Widget(....)\n _startpoint = " << _startpoint.X << "," << _startpoint.Y << "\n";
	        //init();
	        sutex = SDLSuTexWrap(filename, rend);
	        current_pos_ = _startpoint;
	        
	    }

	    SDL_Rect rect() const {
	        return SDL_Rect{static_cast<int>(_startpoint.X), static_cast<int>(_startpoint.Y), _width, _height};
	    }
	    
	    void make(Point p, int w, int h) {
	        //std::cout << "make(Point, int, int)\n";
	        _startpoint = p;
	        _width = w;
	        _height = h;
	        _rect = {static_cast<int>(_startpoint.X), static_cast<int>(_startpoint.Y), _width, _height};
	    }
	    void make() {
	        //std::cout << "make()\n";
	        _rect = {static_cast<int>(_startpoint.X), static_cast<int>(_startpoint.Y), _width, _height};
	    }
	    void show() {
	        //std::cout << "show()\n";
	       
            SDL_RenderCopyEx(sutex.renderer(), sutex.texture(), NULL, &_rect,_angle,nullptr,SDL_FLIP_NONE);	        
	    }

		int rot_angle()  { //rotating angle
		    return _angle;
		}
		void set_rot_angle(int a) {
			_angle = a;
		}
		bool is_rotating() const {
		    return _rotating;
		}
		void set_rotating(bool r) {
		    _rotating = r;
		}
		Point current_pos() const {
			return current_pos_;
		}
		
		Point startpoint() const {
		    return _startpoint;
		}
		void startpoint(int x, int y)  {
		    _startpoint.X = x;
		    _startpoint.Y = y;
		}

		int& width()  {
		    return _width;
		}

		int height() const {
		    return _height;
		}
		void set_height(int h) { _height = h;}
		
		double deltaX() const {
		    return _deltaX;
		}
		int deltaY() const {
		    return _deltaY;
		}
		void set_deltaX(int x) {
		    _deltaX = x;
		}
		void set_deltaY(int y) {
		    _deltaY = y;
		}
		void moveTo(int x, int y) {
		    startpoint(x, y);
		    current_pos_ = _startpoint;
		    make();
		    show();
			//std::cout << "y = " << y << "\n";
		}
		void moveTo(Point p) {
		    moveTo(p.X, p.Y);
		}
		
		friend bool operator==(const Widget& w1, const Widget& w2);
		friend bool operator!=(const Widget& w1, const Widget& w2);
	
};
bool operator==(const Widget& w1, const Widget& w2) {//TODO: B�r kanskje sjekke mere enn bare X,Y
    return (w1._startpoint.X == w2._startpoint.X && w1._startpoint.Y == w2._startpoint.Y) && 
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
        //std::cout << "Pang side \n F�r set_DeltaX w1.deltaX() == " << w1.deltaX() << "\n ";
        w1.set_deltaX(-w1.deltaX());
        w2.set_deltaX(-w2.deltaX());
        w1.set_rotating(true);// == true;
        w2.set_rotating(true);// == true;
        move_widget(w1);
        move_widget(w2);
     
    }
    if(widget_top_bottom_crash(w1, w2) || widget_top_bottom_crash(w2, w1)) {
        //std::cout << "Pang top/bottom \n F�r set_DeltaY w1.deltaY() == " << w1.deltaY() << "\n "; 
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
void handle_top_bottom_crash(Widget& w, int windows_height) {
    if(w.startpoint().Y < 0 || w.startpoint().Y > (windows_height - w.height())) {
         //w.deltaY() = -w.deltaY();
         w.set_deltaY(-w.deltaY());
    }
}
void handle_side_crash(Widget& w, int windows_width) {
    if(w.startpoint().X < 0 || w.startpoint().X > (windows_width - w.width())) {
        //w.deltaX() = -w.deltaX();
        w.set_deltaX(-w.deltaX());
    }
}
#endif


