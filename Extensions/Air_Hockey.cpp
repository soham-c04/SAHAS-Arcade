#include <graphics.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "Screen.h"
#include "Air_Hockey.h"
using namespace std;

const double pi=2*acos(0.0);
double rad(int degree){ // converts degree to radian
	return pi*degree/180.0;
}
double deg(double radi){// converts radian to degree
	return 180.0*radi/pi;
}

int X1=0,Y1=0; //coordinates of top left corner of playable field
int X2=0,Y2=0; //coordinates of bottom right corner of playable field
int X=10,Y=100*Yratio; // Non-playable part of field

int Time=0;  // Time counter

int check_collision(); // Check for collision

class Striker{
	public:
		const int radius=50; // Radius of striker
		const int speed=10;  // Fixed speed of striker (when controlled)
		int x,y; 	  // Coords of strikers
		int color; 	  // Color of strikers

		void reset(int who){ // After scoring a goal reset back to initial values
			if(who==1) x=X1+radius+10;	// Reset striker1 x coordinate
			else x=X2-radius-10;        // Reset striker2 x coordinate
			y=(Y2+Y1)/2; 				// Reset strikers y coordinate
		}

		void control(int who){ // Controlling strikers using keyboard
			int up='W',left='A',down='S',right='D';
			if(who==2){
				up=VK_UP; down=VK_DOWN;
				left=VK_LEFT; right=VK_RIGHT;
			}

			int incx=0,incy=0;
			if(GetAsyncKeyState(up)) incy=speed;
			if(GetAsyncKeyState(down)) incy=-speed;
			if(GetAsyncKeyState(left)) incx=-speed;
			if(GetAsyncKeyState(right)) incx=speed;

			x+=incx;
			y-=incy;

			y=max(y,Y1+radius);
			y=min(y,Y2-radius);
			if(who==1){
				x=max(x,X1+radius);
				x=min(x,(X2+X1)/2-radius);
			}
			else{
				x=max(x,(X2+X1)/2+radius);
				x=min(x,X2-radius);
			}

			if(check_collision()>=1){
				x-=incx;
				y+=incy;
			}
		}
}striker1,striker2;

class Ball{
	private:
		const int start_speed=20; // Fixed speed of ball
	public:
		const int radius=25; // Radius of ball
		int speed=20;   	 // Speed of ball (slowed when goal occurs)
		int x,y; 			 // Coords of ball
		double angle=90.0; 	 // Angle at which the ball is currently moving w.r.t +ve x-axis anticlockwise
		
		void reset(){ // After scoring a goal reset back to center
			speed=start_speed; // Reset ball speed to initial value
			x=(X2+X1)/2;  // reset ball X coordinate to middle of playable field
			y=(Y2+Y1)/2;  // reset ball Y coordinate to middle of playable field
			angle=90.0;   // moves ball vertically next round so both players get fair chance to score
		}
		
		void move(bool Goaling=false);  // Goaling check goal condition
		void boundary();				// Checks for boundary collisions and goals
		void collision(Striker s){
			if(pow(s.x-x,2)+pow(s.y-y,2)<=pow(s.radius+radius,2)){
				double theta=deg(atan2(s.y-y,x-s.x));

				double anglee=angle;
				angle=fmod(fmod(180.0+2*theta-angle,360.0)+360.0,360.0);

				int incx=round(speed*cos(rad(anglee))), incy=round(speed*sin(rad(anglee)));
				if(fabs(anglee-angle)>10.0){
					x-=incx;
					y+=incy;
				}
				else move(); //Trying to debug tangential collisions with striker moving towards them
			}
		}
}ball;

int check_collision(){ // Function to check collision of ball and strikers
	int ans=0;
	// Collision of ball with first striker
	if(pow(striker1.x-ball.x,2)+pow(striker1.y-ball.y,2)<pow(striker1.radius+ball.radius,2)) ans++;
	// Collision of ball with second striker
	if(pow(striker2.x-ball.x,2)+pow(striker2.y-ball.y,2)<pow(striker1.radius+ball.radius,2)) ans+=2;
	// Collision of one striker with another
	if(pow(striker1.x-striker2.x,2)+pow(striker1.y-striker2.y,2)<=pow(2*striker1.radius,2)) ans+=3;

	return ans;
}

void reset(){ // Reset everything to initial values after goal
	ball.reset();
	striker1.reset(1);
	striker2.reset(2);
}

void print(); // Prints everything

class Goal{
	public:
		const int width=90*Xratio;
		const int height=125*Yratio;
		int goal_1=0,goal_2=0;  // goal counter for first and second player respectively

  		void goaling(int who){ //checks goal condition and updates value if goal is scored
			ball.speed=3; // Slow down for goal animation
			if(who==1){ // gone into player 1's net
				goal_2++; // Player 2 scores
				while(ball.x>(X1-3*width/4) && ball.y>=((Y2+Y1)/2-height+ball.radius) && ball.y<=((Y2+Y1)/2+height-ball.radius)){ // Continue animation till be ball reaches any edge of the net
					print();
					ball.move(true);
					delay(10);
				}
			}
			else{
				goal_1++; //player 1 scores
				while(ball.x<(X2+3*width/4) && ball.y>=((Y2+Y1)/2-height+ball.radius) && ball.y<=((Y2+Y1)/2+height-ball.radius)){
					print();
					ball.move(true);
					delay(10);
				}
			}

			reset();

			setvisualpage(1-page);
			readimagefile("resources/Air_Hockey/goal.jpg",(X1+X2)/2-150*Xratio,(Y1+Y2)/2-75*Yratio,(X1+X2)/2+150*Xratio,(Y1+Y2)/2+75*Yratio); // Print GOAL in the center of screen
			settextstyle(4,HORIZ_DIR,3*Xratio); //set text style to previous one
			sleep(2);
		}
}goal;

void Full_Reset(){ // Restarts whole game
	reset();
	goal.goal_1=0;
	goal.goal_2=0;
	Time=0;
}

void Ball::move(bool Goaling){
	int incx=round(speed*cos(rad(angle))); // X component of speed of ball
	int incy=round(speed*sin(rad(angle))); // Y component of speed of ball
	x+=incx; // increment in X coordinate of ball
	y-=incy; // increment in Y coordinate of ball
	if(!Goaling){
		if(y<((Y2+Y1)/2-goal.height+radius) || y>((Y2+Y1)/2+goal.height-radius)){
			x=max(x,X1+radius);
			x=min(x,X2-radius);
		}
		y=max(y,Y1+radius);
		y=min(y,Y2-radius);
		
		boundary();
	}
}

void Ball::boundary(){
	if(x<=(radius+X1) && y>=((Y2+Y1)/2-goal.height+radius) && y<=((Y2+Y1)/2+goal.height-radius)){
		goal.goaling(1); // Ball in left goal
		return;
	}
	else if((x+radius)>=X2 && y>=((Y2+Y1)/2-goal.height+radius) && y<=((Y2+Y1)/2+goal.height-radius)){
		goal.goaling(2); // Ball in right goal
		return;
	}
	// Collision with left,right and top,down boundaries
	if(x<=(radius+X1) || (x+radius)>=X2) angle=fmod(fmod(180.0-angle,360.0)+360.0,360.0);
	if(y<=(radius+Y1) || (y+radius)>=Y2) angle=fmod(fmod(360.0-angle,360.0)+360.0,360.0);
}

void settings(){
	setvisualpage(page);
	cleardevice();
	setcolor(1);
	setlinestyle(0,0,5);
	rectangle(50,50,width-50,height-50);
	setfillstyle(SOLID_FILL,8);
	floodfill(width/2,height/2,1);
	char c1[]="Choose color for PLAYER 1:";
	settextstyle(3,HORIZ_DIR,7);
	outtextxy(width/4-100,100,c1);//prints text for player 1 to choose color
	char c2[]="Press 1 to choose";
	settextstyle(3,HORIZ_DIR,4);
	outtextxy(width/4-75,height-150,c2);
	char c3[]="Press 2 to choose";
	outtextxy(3*width/4-225,height-150,c3);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,1);//color choice 1
	circle(width/4+75,height/2+25,striker1.radius*2.5);//prints first circle
	floodfill(width/4+75,height/2+25,WHITE);//fills circle 1 with first color choice
	circle(3*width/4-75,height/2+25,striker1.radius*2.5);//prints second circle
	setfillstyle(SOLID_FILL,3);//color choice 2
	floodfill(3*width/4-75,height/2+25,WHITE);//fills circle 2 with second color choice
	setvisualpage(page);
	setactivepage(1-page);
	page=1-page;
	
	while(true){
		if(GetAsyncKeyState('1')){
			striker1.color=1; // assigns color choice 1 to striker 1
			break;
		}
		else if(GetAsyncKeyState('2')){
			striker1.color=3; // assigns color choice 2 to striker 1
			break;
		}
		delay(10);
	}
	
	while(GetAsyncKeyState('1') || GetAsyncKeyState('2')) delay(10);//to avoid misckicking of keys
	
	cleardevice();
	setcolor(1);
	setlinestyle(0,0,5);
	rectangle(50,50,width-50,height-50);//outer border
	setfillstyle(SOLID_FILL,8);
	floodfill(width/2,height/2,1);
	char c4[]="Choose color for PLAYER 2:";//prints text for player 2 to chose color
	settextstyle(3,HORIZ_DIR,7);
	outtextxy(width/4-100,100,c4);
	char c5[]="Press 1 to choose";
	settextstyle(3,HORIZ_DIR,4);
	outtextxy(width/4-75,height-150,c5);
	char c6[]="Press 2 to choose";
	outtextxy(3*width/4-225,height-150,c6);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,4);//color choice 1
	circle(width/4+75,height/2+25,striker1.radius*2.5);//prints first circle
	floodfill(width/4+75,height/2+25,WHITE);//fills circle 1 with first color choice
	circle(3*width/4-75,height/2+25,striker1.radius*2.5);//prints second cirlce
	setfillstyle(SOLID_FILL,5);//color choice 2
	floodfill(3*width/4-75,height/2+25,WHITE);//fills circle 2 with second color choice
	setvisualpage(page);
	setactivepage(1-page);
	page=1-page;
	
	while(true){
		if(GetAsyncKeyState('1')){
			striker2.color=4; // assigns color choice 1 to striker 2
			break;
		}
		else if(GetAsyncKeyState('2')){
			striker2.color=5; // assigns color choice 2 to striker 2
			break;
		}
		delay(10);
	}
	settextstyle(4,HORIZ_DIR,3*Xratio);
}

void text(){//prints texts while game is running
	char g1[3]="00";
	char g2[3]="00";
	char c1[]="TIME",c2[]="PLAYER 1",c3[]="PLAYER 2",c4[]="Press Escape",c5[]="for Menu",c6[]="Press Escape",c7[]="for Menu";
		g1[1]=(char)(goal.goal_1%10+'0');
		g1[0]=(char)(goal.goal_1/10+'0');
		g2[1]=(char)(goal.goal_2%10+'0');
		g2[0]=(char)(goal.goal_2/10+'0');
	outtextxy((X1+X2)/2-108*Xratio,30*Yratio,g1);
	outtextxy((X1+X2)/2+70*Xratio,30*Yratio,g2);
	char t[4];
		sprintf(t,"%d",Time);
		int tme=Time,sz=0;
	    while(tme>0){
			tme/=10;
			sz++;
		}
		sz=max(sz,1);
		outtextxy((X1+X2)/2-42*Xratio,5*Yratio,c1);
		outtextxy((X1+X2)/2-10*sz*Xratio,38*Yratio,t);
		outtextxy((X1+X2)/2-304*Xratio,28*Yratio,c2);
		outtextxy((X1+X2)/2+135*Xratio,28*Yratio,c3);
	outtextxy(X,18*Yratio,c4);
	outtextxy(X,42*Yratio,c5);
	outtextxy(width-230*Xratio,18*Yratio,c6);
	outtextxy(width-150*Xratio,42*Yratio,c7);
}

void print_movable(){
	setcolor(15);
		circle(striker1.x,striker1.y,striker1.radius);
		circle(striker2.x,striker2.y,striker1.radius);
		circle(ball.x,ball.y,ball.radius);
	setfillstyle(SOLID_FILL,striker1.color);
		floodfill(striker1.x,striker1.y,15);
	setfillstyle(SOLID_FILL,striker2.color);
		floodfill(striker2.x,striker2.y,15);
	setfillstyle(SOLID_FILL,14);
		floodfill(ball.x,ball.y,15);
}

void color(){
	setfillstyle(SOLID_FILL,2);
		floodfill(X1+2,Y1+2,BLACK);
		floodfill(X2-2,Y2-2,BLACK);
	setfillstyle(SOLID_FILL,9);
		floodfill(X1-10,(Y+height)/2,BLACK);
		floodfill((X1+X2)/2-10,(Y+height)/2,BLACK);
		floodfill((X1+X2)/2-100*Xratio,40*Yratio,BLACK);
	setfillstyle(SOLID_FILL,12);
		floodfill(X2+10,(Y+height)/2,BLACK);
		floodfill((X1+X2)/2+10,(Y+height)/2,BLACK);
		floodfill((X1+X2)/2+100*Xratio,40*Yratio,BLACK);
	setfillstyle(4,BLACK);//bleachers
		floodfill(X1-2,Y1+2,BLACK);
		floodfill(X1-2,Y2-2,BLACK);
		floodfill(X2+2,Y1+2,BLACK);
		floodfill(X2+2,Y2-2,BLACK);
}

void print_field(){
	cleardevice();
	setcolor(BLACK);
	setlinestyle(0,0,3);
	//making stadium
		rectangle(X,Y,width,height);
		rectangle(X,(Y+height)/2-goal.height,X+goal.width,(Y+height)/2+goal.height);//making left goal
		rectangle(width-goal.width,(Y+height)/2-goal.height,width,(Y+height)/2+goal.height);//making right goal
		rectangle(X1,Y1,X2,Y2);//making playable field
		
	line((X1+X2)/2,Y1,(X1+X2)/2,Y2);//central line
	circle((X1+X2)/2,(Y1+Y2)/2,100);//making central circle
	
	rectangle((X1+X2)/2-50*Xratio,0,(X1+X2)/2+50*Xratio,80*Yratio);//time square
	rectangle(((X1+X2)/2-50*Xratio)-80*Xratio,10,(X1+X2)/2-50*Xratio,70*Yratio);//score square 1
	rectangle(((X1+X2)/2+50*Xratio)+80*Xratio,10,(X1+X2)/2+50*Xratio,70*Yratio);//score square 2
	rectangle(((X1+X2)/2-50*Xratio)-265*Xratio,20,(X1+X2)/2-130*Xratio,60*Yratio);//name square 1
	rectangle(((X1+X2)/2+50*Xratio)+265*Xratio,20,(X1+X2)/2+130*Xratio,60*Yratio);//name square 2
}

void print(){
	setactivepage(page);
	setvisualpage(1-page);
	print_field();
	color();
	text();
	print_movable();
	page=1-page;
}

void pause_screen(int option){
	setactivepage(page);
	setvisualpage(1-page);
	switch(option){
		case 1:
			readimagefile("resources/Air_Hockey/pause_screen_resume.jpg",(X1+X2)/2-200*Xratio,(Y1+Y2)/2-200*Yratio,(X1+X2)/2+200*Xratio,(Y1+Y2)/2+200*Yratio);
			break;
		case 2:
			readimagefile("resources/Air_Hockey/pause_screen_reset.jpg",(X1+X2)/2-200*Xratio,(Y1+Y2)/2-200*Yratio,(X1+X2)/2+200*Xratio,(Y1+Y2)/2+200*Yratio);
			break;
		case 3:
			readimagefile("resources/Air_Hockey/pause_screen_settings.jpg",(X1+X2)/2-200*Xratio,(Y1+Y2)/2-200*Yratio,(X1+X2)/2+200*Xratio,(Y1+Y2)/2+200*Yratio);
			break;
		case 4:
			readimagefile("resources/Air_Hockey/pause_screen_quit.jpg",(X1+X2)/2-200*Xratio,(Y1+Y2)/2-200*Yratio,(X1+X2)/2+200*Xratio,(Y1+Y2)/2+200*Yratio);
			break;
		default:
			readimagefile("resources/Air_Hockey/pause_screen_resume.jpg",(X1+X2)/2-200*Xratio,(Y1+Y2)/2-200*Yratio,(X1+X2)/2+200*Xratio,(Y1+Y2)/2+200*Yratio);
	}
	page=1-page;
}

bool pause(){
	int option=1;
	while(true){
		if(GetAsyncKeyState(VK_UP)){
			option--;
			if(option<1) option=4;
		}
		else if(GetAsyncKeyState(VK_DOWN)){
			option++;
			if(option>4) option=1;
		}
		else if(GetAsyncKeyState(VK_RETURN)){
			switch(option){
				case 1:
					return true;
				case 2:
					Full_Reset();
					return true;
				case 3:
					page=1-page;
					settings();
					return true;
				case 4:
					return false;
				default:
					return true;
			}
		}
		else if(GetAsyncKeyState(VK_ESCAPE)){	
			while(GetAsyncKeyState(VK_ESCAPE));
			return true;
		}
		pause_screen(option);
		delay(100);
	}
}

void game(){
	time_t ti,tf;
	time(&ti);
	while(true){
		time(&tf);
		if(ti!=tf){//incrementing time
			ti=tf;
			Time++;
		}
		
		// Printing game screen
		print();
		
		// Physics
		ball.move();
		ball.collision(striker1);
		ball.collision(striker2);
		striker1.control(1);
		striker2.control(2);
		
		if(GetAsyncKeyState(VK_ESCAPE)){
			while(GetAsyncKeyState(VK_ESCAPE));
			bool run_game=pause();
			if(!run_game) break;
		}
		delay(10);
	}
}

void start_AH(){
	height-=20;
	width-=10;
	
	X1=X+goal.width; 	 Y1=Y;
	X2=width-goal.width; Y2=height;
	Full_Reset();
	
	cleardevice();
	setbkcolor(7);
	settings();
	game();
	
	height+=20;
	width+=10;
}
