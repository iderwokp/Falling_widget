#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include "sdl_wrap.h"
#include "Falling_widget.h"
#include "vec2d.h"

using namespace Iderwok;

std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde);
float grav_avstand(double avstand, float g);
void check_limits(Falling_widget& romskip, int wwidth, int wheight, int fwidget_width, int fwidget_height);
void animate(Falling_widget& fw, const std::array<std::string, 2>& s);
const int aksellerasjons_justering{100};
const int trust_vs_aksjust{2};
//int mouse_x{0};
//int mouse_y{0};
int rot_angle{0};
double trust{0};
bool changeSpr{false};
void EventHandler(SDL_Event event, bool& quit) {//, int ww, int wh) {
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT){
        quit = true;
    }
	else if(event.type == SDL_KEYDOWN) {
	        if(event.key.keysym.sym == SDLK_RIGHT ) {
	            rot_angle+=5;
	            if(rot_angle > 360) rot_angle -=360;
	        }
	        if(event.key.keysym.sym == SDLK_LEFT ) {
	            rot_angle-=5;
	            if(rot_angle < 0) rot_angle +=360;
	        }
	        if(event.key.keysym.sym == SDLK_UP ) {
	            trust = 9.81/(aksellerasjons_justering/trust_vs_aksjust);
	            changeSpr = true;
	        }
	        if(event.key.keysym.sym == SDLK_DOWN ) {

	        }
	}
    else if(event.type == SDL_KEYUP) {
        	if(event.key.keysym.sym == SDLK_UP ) {
           	 trust = 0.0;
           	 changeSpr = false;
       		 }
       		 if(event.key.keysym.sym == SDLK_DOWN ) {
           		 rot_angle += 180;
        }
    }


//    if(event.type == SDL_MOUSEBUTTONDOWN) {
//
//        SDL_GetMouseState( &mouse_x, &mouse_y );
//        //check_mouse_click(x, y, w, ww, wh);
//
//    }
}
int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) {

	SDL_Event event;
	bool quit{false};
    const int windows_width {1300};
    const int windows_height {600};

    SDL_Init(SDL_INIT_VIDEO);

    Sdl_wrap sdlwrap{std::string{"Romskip"}, windows_width, windows_height};
    //SDL_Window* window = sdlwrap.window();
    SDL_Renderer* renderer = sdlwrap.renderer();

    int fwidget_width{17};
    int fwidget_height{30};
    //std::vector<Falling_widget> baller;
    //baller.emplace_back("ball.bmp", renderer, Point{750.0, 350.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0, -2, 0);

    Falling_widget romskip("ball2.bmp", renderer, Point{850.0, 350.0}, fwidget_width, fwidget_height, windows_height, windows_width, 0, 0, 0);
    std::array<std::string, 2> sprites = {"ball2.bmp", "ball2_2.bmp"};//, "ball2_3.bmp"};

    Vec2d<double> tyngdekraft{0.0, 9.81/aksellerasjons_justering};



    double midwinX = windows_width/2;
    double midwinY = windows_height/2;

    int index{300};

    romskip.setXY(midwinX-fwidget_width/2, midwinY-fwidget_height/2);
    //romskip.setXY(midwinX, midwinY);
    while(index >=0 && !quit) {

        EventHandler(event, quit);//, windows_width, windows_height);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        check_limits(romskip, windows_width, windows_height, fwidget_width, fwidget_height);

	    romskip.updateXY();
	    romskip.set_rot_angle(rot_angle);
	    //Aksellerasjon akslr(trust, rot_angle-90);
	    Vec2d<double> aksvec(trust, (int)rot_angle-90);
	   // Vec2d<double> aksvec(akslr.X(), akslr.Y());
	   // std::cout << "aksvec.x() = " << aksvec.xVal() << "\taksvec.y() = " << aksvec.yVal() << "\n";
	    Vec2d<double> tot_aks = aksvec + tyngdekraft;
	   // std::cout << "tot_aks.xVal() = " << tot_aks.xVal() << "\ttot_aks.yVal() = " << tot_aks.yVal() << "\n\n";
	    romskip.set_aksellerasjon(tot_aks.xVal(), static_cast<float>(tot_aks.yVal()));
	    if(changeSpr) animate(romskip, sprites);
	    else romskip.change_sprite("ball2.bmp");
	    //std::cout << "velocityTot() = " << romskip.velocityTot() << "\n";
	    //std::cout << "romskip.current_pos().X = " << romskip.current_pos().X << "\tromskip.current_pos().Y = " << romskip.current_pos().Y << "\n";
	    //++rot_angle;


		SDL_RenderPresent(renderer);
		//SDL_RenderClear(renderer);
		SDL_Delay(10);



    }

    SDL_Quit();
    return 0;
}

void animate(Falling_widget& fw, const std::array<std::string, 2>& s) {
	static int counter{0};
	fw.change_sprite(s[counter]);
	++counter;
	if (counter == 2) counter = 0;

}


void check_limits(Falling_widget& romskip, int windows_width, int windows_height, int /*fwidget_width*/, int fwidget_height) {
//En slags samleprosedyre
//Tar seg av hva som skjer når romskipet kommer utenfor kanten eller kjører veldig sakte
//Ikke helt stuerent å ha flere oppgaver i en funksjon men...
	double loss{0.5}; //Evnen til å sprette
	double safe_speed{5.0};
	int yPos = romskip.current_pos().Y;
    int xPos = romskip.current_pos().X;
    if (xPos < 0) 			   	{romskip.setXY(windows_width, yPos); }
    if (xPos > windows_width)	 {romskip.setXY(0, yPos); }
    //if (yPos < 0) 				{romskip.setXY(xPos, windows_height);}
    if (yPos > windows_height-fwidget_height)	 {
    	if(romskip.velocityTot() > safe_speed) std::cout << "CHRASH!!\n";
		romskip.setXY(xPos, windows_height-fwidget_height);romskip.set_velocityY(-romskip.velocityY()*loss);romskip.set_velocityX(romskip.velocityX()*loss);
	}
//    if(fabs(romskip.velocityX()) < 0.005 && trust == 0.0) romskip.set_velocityX(0.0);
//    if(fabs(romskip.velocityY()) < 0.005 && trust == 0.0) romskip.set_velocityY(0.0);
}

std::pair<double, double> endKoord(double startX, double startY, double rad_vinkel, double lengde) {

	double x = lengde*cos(rad_vinkel);
	double y = lengde*sin(rad_vinkel);
	return std::make_pair(x+startX, y+startY);

}

float grav_avstand(double avstand, float g) {
	if(avstand< 5.0f) avstand = 5.0f;
	return g/(avstand*avstand);

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
 //       }

        // Smågetoppen grendalag: 98656042

//       const int marg{10};
//        SDL_RenderDrawLine(renderer, midwinX,midwinY-marg, midwinX, midwinY+marg);//Loddrett
//		SDL_RenderDrawLine(renderer, midwinX-marg,midwinY, midwinX+marg,midwinY);//Vannrett

