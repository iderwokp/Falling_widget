#include <SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "sdl_wrap.h"
#include "Falling_widget.h"
#include "vec2d.h"

using namespace Iderwok;

std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde);
float grav_avstand(double avstand, float g);
int mouse_x{0};
int mouse_y{0};
bool clearscreen{false};
void EventHandler(SDL_Event event, bool& quit) {//, int ww, int wh) {
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT){
        quit = true;
    }    
    if(event.type == SDL_MOUSEBUTTONDOWN) {
        clearscreen = true;
        SDL_GetMouseState( &mouse_x, &mouse_y );
        //check_mouse_click(x, y, w, ww, wh);
        
    }
}
int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) {
	
	SDL_Event event;
	bool quit{false};
    const int windows_width {1300};
    const int windows_height {700};
    
    SDL_Init(SDL_INIT_VIDEO);
    
    Sdl_wrap sdlwrap{std::string{"Falling"}, windows_width, windows_height};
    //SDL_Window* window = sdlwrap.window();
    SDL_Renderer* renderer = sdlwrap.renderer();
    const float speed_konstant{0.1};
    int fwidget_width{30};
    int fwidget_height{20};
    std::vector<Falling_widget> baller;
    //baller.emplace_back("ball.bmp", renderer, Point{750.0, 350.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0, -2, 0);
    
    baller.emplace_back("ball.bmp", renderer, Point{500.0, 133.0}, fwidget_width+10, fwidget_height+10, windows_height, windows_width, -0*speed_konstant, -0*speed_konstant, 0);
    baller.emplace_back("ball.bmp", renderer, Point{500.0, 433.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0); 
 //   baller.emplace_back("ball.bmp", renderer, Point{800.0, 600.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{550.0, 270.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{650.0, 150.0}, fwidget_width, fwidget_height, windows_height, windows_width, -0*speed_konstant, 0*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{650.0, 550.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0*speed_konstant, 0*speed_konstant, 0);
////    
//    baller.emplace_back("ball.bmp", renderer, Point{750.0, 450.0}, fwidget_width, fwidget_height, windows_height, windows_width, 15*speed_konstant, -15*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{630.0, 555.0}, fwidget_width, fwidget_height, windows_height, windows_width, 34*speed_konstant, 1*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{444.0, 333.0}, fwidget_width, fwidget_height, windows_height, windows_width, -5*speed_konstant, 33*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{800.0, 50.0}, fwidget_width, fwidget_height, windows_height, windows_width, -36*speed_konstant, -15*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{250.0, 100.0}, fwidget_width, fwidget_height, windows_height, windows_width, -33*speed_konstant, 33*speed_konstant, 0);
//    baller.emplace_back("ball.bmp", renderer, Point{300.0, 70.0}, fwidget_width, fwidget_height, windows_height, windows_width, -29*speed_konstant, 29*speed_konstant, 0);
//    

    
 //   double midwinX = windows_width/2;
 //   double midwinY = windows_height/2;
//    int sjerneW{16};
//    int sjerneH{10};
//    Widget sjerne("ball.bmp", renderer, Point{midwinX-sjerneW/2, midwinY-sjerneH/2},sjerneW, sjerneH, 0, 0, 0);
    //double xPos, yPos;
    //double length_vecXY, radangl;
    int index{300};
    float gravitasjon{0.0981*speed_konstant*speed_konstant};
    //float gravitasjon2{9.81/1500};
    while(index >=0 && !quit) {
	
        EventHandler(event, quit);//, windows_width, windows_height);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        if(clearscreen) {
        	SDL_RenderClear(renderer); 
			clearscreen = false;	
        }
        SDL_RenderClear(renderer); 
        Vec2d<double> x_axe{500.0, 0.0}; 				//Vektor som representerer x-aksen. Vinkelen er mellom x_axe og vecXY
        for(auto& gjeldende_ball: baller) {
        	double xPos = gjeldende_ball.current_pos().X;
        	double yPos = gjeldende_ball.current_pos().Y;
        	Vec2d<double> acc_vec{0.0, 0.0};
        	Vec2d<double> start_vec {xPos, yPos};
        	//Vec2d<double> justerings_vec {0.1, 180};
        	
	        double sjekke_mot_ball_Y{0};
        	for(const auto& sjekke_mot_ball: baller) {
        		if(gjeldende_ball == sjekke_mot_ball) continue;
        		double xPos2 = sjekke_mot_ball.current_pos().X;
        		double yPos2 = sjekke_mot_ball.current_pos().Y;
        		
        		Vec2d<double> sjekke_mot_ball_vec{xPos2, yPos2};
        		acc_vec += (sjekke_mot_ball_vec);// - start_vec);
        		//acc_vec += justerings_vec;
        		
        	}
        	
        	Vec2d<double> vecXY = acc_vec - start_vec; 		//Vektor som representerer fra ball til gravitasjonspunktet
        	
        	sjekke_mot_ball_Y = vecXY.yVal();
        	double length_vecXY = vecXY.length();
	        double justert_lengde = length_vecXY/10;
	        //Gravitasjonsvektor
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	        SDL_RenderDrawLine(renderer, xPos,yPos, (vecXY+ start_vec).xVal(), (vecXY+ start_vec).yVal());
	        //std::cout << "xPos=" << xPos << ", yPos=" << yPos << " -> acc_vec.xVal()=" << acc_vec.xVal() << ", acc_vec.yVal()=" << acc_vec.yVal() << std::endl;
			
			double angle = angle_deg(vecXY, x_axe);
			if (gjeldende_ball.current_pos().Y > sjekke_mot_ball_Y) angle *= -1;
	        	        
	        float grav_rr = grav_avstand(justert_lengde, gravitasjon);
	        gjeldende_ball.set_aksellerasjon(grav_rr, static_cast<int>(angle));
	        gjeldende_ball.updateXY();
        }
//        for(auto& ball: baller) {
//        	double xPos = ball.current_pos().X;
//        	double yPos = ball.current_pos().Y;
//        	
//        	Vec2d<double> midwin{midwinX, midwinY};		    //Vektor som representerer gravitasjonspunktet
//      		Vec2d<double> pos_vec{xPos, yPos};				//Vektor som representerer posisjonen til ballen
//	        Vec2d<double> vecXY = midwin - pos_vec; 		//Vektor som representerer fra ball til gravitasjonspunktet
//	        Vec2d<double> x_axe{500.0, 0.0}; 				//Vektor som representerer x-aksen. Vinkelen er mellom x_axe og vecXY
//			
//			double length_vecXY = vecXY.length();
//	        double justert_lengde = length_vecXY/100;
//			
//			int angle = static_cast<int>(angle_deg(vecXY, x_axe));
//			if (ball.current_pos().Y > midwinY) angle *= -1;
//	        	        
//	        float grav_rr = grav_avstand(justert_lengde, gravitasjon);
//	        ball.set_aksellerasjon(grav_rr, angle);
//	        ball.updateXY();
//
//	        if(mouse_x != 0 || mouse_y != 0) {
//	        	midwinX = mouse_x;
//	        	midwinY = mouse_y;
//	        }
	        //Tegne gravitasjonslinjer
//	        double radangl = angle_rad(vecXY, x_axe);//For gravitasjonslinjene
//	        if (ball.current_pos().Y > midwinY) radangl *= -1; //For gravitasjonslinjene
//	        auto [endX, endY] = endKoord(xPos, yPos, radangl, length_vecXY);
//	        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
//	        SDL_RenderDrawLine(renderer, xPos,yPos, endX, endY);
//        }

        // Smågetoppen grendalag: 98656042
        
//       const int marg{10};
//        SDL_RenderDrawLine(renderer, midwinX,midwinY-marg, midwinX, midwinY+marg);//Loddrett
//		SDL_RenderDrawLine(renderer, midwinX-marg,midwinY, midwinX+marg,midwinY);//Vannrett
	    
	   // sjerne.moveTo(midwinX-sjerneW/2, midwinY-sjerneH/2);
	    
        
		
		SDL_RenderPresent(renderer);
		//SDL_RenderClear(renderer); 
		SDL_Delay(10); 
       
       
       
    }
   
    SDL_Quit();
    return 0;
}

std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde) {
	
	double x = lengde*cos(rad_vinkel);
	double y = lengde*sin(rad_vinkel);
	return std::make_pair(x+startX, y+startY);
	
}

float grav_avstand(double avstand, float g) {
	if(avstand< 5.0f) avstand = 5.0f;
	//return g/(avstand*avstand);
	return g;
}



//        int ypos = fw.current_pos().Y;
//        int xpos = fw.current_pos().X;
//        int ypos2 = fw2.current_pos().Y;
//        int xpos2 = fw2.current_pos().X;
//        int ypos3 = fw3.current_pos().Y;
//        int xpos3 = fw3.current_pos().X;
//        double vecX = midwinX - xpos;
//        double vecY = midwinY - ypos;
//        double vecX2 = midwinX - xpos2;
//        double vecY2 = midwinY - ypos2;
//        double vecX = xpos2 - xpos;
//        double vecY = ypos2 - ypos;
//        double vecX2 = xpos - xpos2;
//        double vecY2 = ypos - ypos2;
//        double vecX3 = xpos2 - xpos3;
//        double vecY3 = ypos2 - ypos3;
//        double length_vecXY = sqrt(vecX*vecX + vecY*vecY);
//        double length_vecXY2 = sqrt(vecX2*vecX2 + vecY2*vecY2);
//        double length_vecXY3 = sqrt(vecX3*vecX3 + vecY3*vecY3);
//        double justert_lengde = length_vecXY/100;
//        double justert_lengde2 = length_vecXY2/100;
//        double justert_lengde3 = length_vecXY3/100;
//        
//        double ekspr = (vecX*vecX+vecY*0)/(length_vecXY*vecX);
//        double ekspr2 = (vecX2*vecX2+vecY2*0)/(length_vecXY2*vecX2);
//        double ekspr3 = (vecX3*vecX3+vecY3*0)/(length_vecXY3*vecX3);
//        double radangl = acos(ekspr);
//        double radangl2 = acos(ekspr2);
//        double radangl3 = acos(ekspr3);
//        if (ypos > ypos2) radangl *= -1;
//        if (ypos2 > ypos) radangl2 *= -1;
//        if (ypos3 > ypos2) radangl3 *= -1;
//        
//        int angle = static_cast<int>(radangl*(180.0/3.1415926));
//        int angle2 = static_cast<int>(radangl2*(180.0/3.1415926));
//        int angle3 = static_cast<int>(radangl3*(180.0/3.1415926));
//        
//        float grav_rr = grav_avstand(justert_lengde, gravitasjon);
//        float grav_rr2 = grav_avstand(justert_lengde2, gravitasjon2);
//        float grav_rr3 = grav_avstand(justert_lengde3, gravitasjon);
//        //std::cout << "grav_rr = " << grav_rr << "\tjustert_lengde^2 = " << justert_lengde * justert_lengde<<"\n";
//        fw.set_aksellerasjon(grav_rr, angle);
//        fw2.set_aksellerasjon(grav_rr2, angle2);
//        fw3.set_aksellerasjon(grav_rr3, angle3);
//        
//        auto [endX, endY] = endKoord(xpos, ypos, radangl, length_vecXY);
//        auto [endX2, endY2] = endKoord(xpos2, ypos2, radangl2, length_vecXY2);
//        auto [endX3, endY3] = endKoord(xpos3, ypos3, radangl3, length_vecXY3);

//SDL_RenderDrawLine(renderer, 0,midwinY, windows_width,midwinY);
		//SDL_RenderDrawLine(renderer, midwinX,0, midwinX, windows_height);
		//SDL_RenderDrawLine(renderer, xpos,ypos, endX, endY);
		//SDL_RenderDrawLine(renderer, xpos2,ypos2, endX2, endY2);
//		fw.updateXY();
//		fw2.updateXY();
//		fw3.updateXY();
        
