#include "Screen.h"
#include "offline.h"
#include <unistd.h>
using namespace std;

int Timer_off=90;  			// Time counter

int check_collision(); 	// Check for collision
void print_off();

struct Striker{
	const int radius=50;			// Radius of striker
	const int speed=10;				// Fixed speed of striker (when controlled)
	int x,y; 	  					// Coords of strikers
	int color; 	  					// Color of strikers

	void reset(int who){			// After scoring a goal reset back to initial values
		if(who==1) x=X1+radius+10;	// Reset striker1 x coordinate
		else x=X2-radius-10;        // Reset striker2 x coordinate
		y=(Y2+Y1)/2; 				// Reset strikers y coordinate
	}

	void control(int who){ 			// Controlling strikers using keyboard
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
}striker1_off, striker2_off;

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
		void collision(Striker &s){
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
}ball_off;

void reset(){ // Reset everything to initial values after goal
	ball_off.reset();
	striker1_off.reset(1);
	striker2_off.reset(2);
}

struct Goal{
	int width, height;
	int goal_1=0, goal_2=0;  // goal counter for first and second player respectively

	void goaling(int who){ //checks goal condition and updates value if goal is scored
		ball_off.speed=3; // Slow down for goal animation
		if(who==1){ // gone into player 1's net
			goal_2++; // Player 2 scores
			while(ball_off.x>(X1-3*width/4) && ball_off.y>=((Y2+Y1)/2-height+ball_off.radius) && ball_off.y<=((Y2+Y1)/2+height-ball_off.radius)){ // Continue animation till be ball reaches any edge of the net
				print_off();
				ball_off.move(true);
				delay(DELAY);
			}
		}
		else{
			goal_1++; //player 1 scores
			while(ball_off.x<(X2+3*width/4) && ball_off.y>=((Y2+Y1)/2-height+ball_off.radius) && ball_off.y<=((Y2+Y1)/2+height-ball_off.radius)){
				print_off();
				ball_off.move(true);
				delay(DELAY);
			}
		}

		reset();

		setvisualpage(1-page);
		readimagefile("resources/Air_Hockey/goal.jpg",(X1+X2)/2-150*Xratio,(Y1+Y2)/2-75*Yratio,(X1+X2)/2+150*Xratio,(Y1+Y2)/2+75*Yratio); // Print GOAL in the center of screen
		settextstyle(4,HORIZ_DIR,3*Xratio); //set text style to previous one
		sleep(2);
	}
}goal_off;

int check_collision(){ // Function to check collision of ball and strikers
	int ans=0;
	// Collision of ball with first striker
	if(pow(striker1_off.x-ball_off.x,2)+pow(striker1_off.y-ball_off.y,2)<pow(striker1_off.radius+ball_off.radius,2)) ans++;
	// Collision of ball with second striker
	if(pow(striker2_off.x-ball_off.x,2)+pow(striker2_off.y-ball_off.y,2)<pow(striker1_off.radius+ball_off.radius,2)) ans+=2;
	// Collision of one striker with another
	if(pow(striker1_off.x-striker2_off.x,2)+pow(striker1_off.y-striker2_off.y,2)<=pow(striker1_off.radius+striker2_off.radius,2)) ans+=3;

	return ans;
}


void Full_Reset(){ // Restarts whole game
	reset();
	goal_off.goal_1=0;
	goal_off.goal_2=0;
	Timer_off=90;
}

void Ball::move(bool Goaling){
	int incx=round(speed*cos(rad(angle))); // X component of speed of ball
	int incy=round(speed*sin(rad(angle))); // Y component of speed of ball
	x+=incx; // increment in X coordinate of ball
	y-=incy; // increment in Y coordinate of ball
	if(!Goaling){
		if(y<((Y2+Y1)/2-goal_off.height+radius) || y>((Y2+Y1)/2+goal_off.height-radius)){
			x=max(x,X1+radius);
			x=min(x,X2-radius);
		}
		y=max(y,Y1+radius);
		y=min(y,Y2-radius);

		boundary();
	}
}

void Ball::boundary(){
	if(x<=(radius+X1) && y>=((Y2+Y1)/2-goal_off.height+radius) && y<=((Y2+Y1)/2+goal_off.height-radius)){
		goal_off.goaling(1); // Ball in left goal
		return;
	}
	else if((x+radius)>=X2 && y>=((Y2+Y1)/2-goal_off.height+radius) && y<=((Y2+Y1)/2+goal_off.height-radius)){
		goal_off.goaling(2); // Ball in right goal
		return;
	}
	// Collision with left,right and top,down boundaries
	if(x<=(radius+X1) || (x+radius)>=X2) angle=fmod(fmod(180.0-angle,360.0)+360.0,360.0);
	if(y<=(radius+Y1) || (y+radius)>=Y2) angle=fmod(fmod(360.0-angle,360.0)+360.0,360.0);
}

void settings_off(){
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
	circle(width/4+75,height/2+25,striker1_off.radius*2.5);//prints first circle
	floodfill(width/4+75,height/2+25,WHITE);//fills circle 1 with first color choice
	circle(3*width/4-75,height/2+25,striker1_off.radius*2.5);//prints second circle
	setfillstyle(SOLID_FILL,3);//color choice 2
	floodfill(3*width/4-75,height/2+25,WHITE);//fills circle 2 with second color choice
	setvisualpage(page);
	setactivepage(1-page);
	page=1-page;

	while(true){
		if(GetAsyncKeyState('1')){
			striker1_off.color=1; // assigns color choice 1 to striker 1
			break;
		}
		else if(GetAsyncKeyState('2')){
			striker1_off.color=3; // assigns color choice 2 to striker 1
			break;
		}
		delay(DELAY);
	}

	while(GetAsyncKeyState('1') || GetAsyncKeyState('2'))
		delay(100);//to avoid misckicking of keys

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
	circle(width/4+75,height/2+25,striker1_off.radius*2.5);//prints first circle
	floodfill(width/4+75,height/2+25,WHITE);//fills circle 1 with first color choice
	circle(3*width/4-75,height/2+25,striker1_off.radius*2.5);//prints second cirlce
	setfillstyle(SOLID_FILL,5);//color choice 2
	floodfill(3*width/4-75,height/2+25,WHITE);//fills circle 2 with second color choice
	setvisualpage(page);
	setactivepage(1-page);
	page=1-page;

	while(true){
		if(GetAsyncKeyState('1')){
			striker2_off.color=4; // assigns color choice 1 to striker 2
			break;
		}
		else if(GetAsyncKeyState('2')){
			striker2_off.color=5; // assigns color choice 2 to striker 2
			break;
		}
		delay(DELAY);
	}
	settextstyle(4,HORIZ_DIR,3*Xratio);
}

void text_off(){//prints texts while game is running
	char g1[3]="00";
	char g2[3]="00";
	char c1[]="TIME LEFT",c2[]="PLAYER 1",c3[]="PLAYER 2",c4[]="Press Escape",c5[]="for Menu",c6[]="Press Escape",c7[]="for Menu";
		g1[1]=(char)(goal_off.goal_1%10+'0');
		g1[0]=(char)(goal_off.goal_1/10+'0');
		g2[1]=(char)(goal_off.goal_2%10+'0');
		g2[0]=(char)(goal_off.goal_2/10+'0');
	outtextxy((X1+X2)/2-108*Xratio-40*Xratio,30*Yratio,g1);
	outtextxy((X1+X2)/2+70*Xratio+45*Xratio,30*Yratio,g2);
	char t[4];
	sprintf(t,"%d",Timer_off);
	int tme=Timer_off,sz=0;
    while(tme>0){
		tme/=10;
		sz++;
	}
	sz=max(sz,1);
	outtextxy((X1+X2)/2-42*Xratio-42*Xratio,7*Yratio,c1);
	outtextxy((X1+X2)/2-10*sz*Xratio,38*Yratio,t);
	outtextxy((X1+X2)/2-304*Xratio-40*Xratio,28*Yratio,c2);
	outtextxy((X1+X2)/2+135*Xratio+45*Xratio,28*Yratio,c3);
	outtextxy(X,18*Yratio,c4);
	outtextxy(X,42*Yratio,c5);
	outtextxy(width-230*Xratio,18*Yratio,c6);
	outtextxy(width-150*Xratio,42*Yratio,c7);
}

void print_movable_off(){
	setcolor(15);
		circle(striker1_off.x,striker1_off.y,striker1_off.radius);
		circle(striker2_off.x,striker2_off.y,striker1_off.radius);
		circle(ball_off.x,ball_off.y,ball_off.radius);
	setfillstyle(SOLID_FILL,striker1_off.color);
		floodfill(striker1_off.x,striker1_off.y,15);
	setfillstyle(SOLID_FILL,striker2_off.color);
		floodfill(striker2_off.x,striker2_off.y,15);
	setfillstyle(SOLID_FILL,14);
		floodfill(ball_off.x,ball_off.y,15);
}

void color_off(){
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

void print_field_off(){
	cleardevice();
	setcolor(BLACK);
	setlinestyle(0,0,3);
	//making stadium
		rectangle(X,Y,width,height);
		rectangle(X,(Y+height)/2-goal_off.height,X+goal_off.width,(Y+height)/2+goal_off.height);//making left goal
		rectangle(width-goal_off.width,(Y+height)/2-goal_off.height,width,(Y+height)/2+goal_off.height);//making right goal
		rectangle(X1,Y1,X2,Y2);//making playable field

	line((X1+X2)/2,Y1,(X1+X2)/2,Y2);//central line
	circle((X1+X2)/2,(Y1+Y2)/2,100);//making central circle

	int c = (X1+X2)/2;
	int x1 = c-355*Xratio, x2 = c-170*Xratio;
	int x3 = c-90*Xratio, x4 = c+90*Xratio;
	int x5 = c+170*Xratio, x6 = c+355*Xratio;
	rectangle(x1, 20*Yratio, x2, 60*Yratio);	// name square 1
	rectangle(x2, 10*Yratio, x3, 70*Yratio);	// score square 1
	rectangle(x3, 00*Yratio, x4, 80*Yratio);	// Time Left square
	rectangle(x4, 10*Yratio, x5, 70*Yratio);	// score square 2
	rectangle(x5, 20*Yratio, x6, 60*Yratio);	// name square 2
}

void print_off(){
	setactivepage(page);
	setvisualpage(1-page);
	print_field_off();
	color_off();
	text_off();
	print_movable_off();
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
					settings_off();
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
	while(Timer_off >= 0){
		time(&tf);
		if(ti!=tf){//incrementing time
			ti=tf;
			Timer_off--;
		}

		// Printing game screen
		print_off();

		// Physics
		ball_off.move();
		ball_off.collision(striker1_off);
		ball_off.collision(striker2_off);
		striker1_off.control(1);
		striker2_off.control(2);

		if(GetAsyncKeyState(VK_ESCAPE)){
			while(GetAsyncKeyState(VK_ESCAPE));
			bool run_game=pause();
			if(!run_game) break;
		}
		delay(DELAY);
	}
	
	setvisualpage(0);
	setactivepage(0);
	if(goal_off.goal_1 > goal_off.goal_2){
		readimagefile("resources/Air_Hockey/player1_wins.jpg",(X1+X2)/2-250*Xratio,(Y1+Y2)/2-250*Yratio,(X1+X2)/2+350*Xratio,(Y1+Y2)/2+150*Yratio);
	}
	else if(goal_off.goal_1 < goal_off.goal_2){
		readimagefile("resources/Air_Hockey/player2_wins.jpg",(X1+X2)/2-250*Xratio,(Y1+Y2)/2-250*Yratio,(X1+X2)/2+350*Xratio,(Y1+Y2)/2+150*Yratio);
	}
	else{
		readimagefile("resources/Air_Hockey/draw.jpg",(X1+X2)/2-250*Xratio,(Y1+Y2)/2-150*Yratio,(X1+X2)/2+350*Xratio,(Y1+Y2)/2+150*Yratio);
	}
	sleep(4);
}

void main_offline(){
	goal_off.width = 90*Xratio;
	goal_off.height= 125*Yratio;
	
	X1=X+goal_off.width; 	 Y1=Y;
	X2=width-goal_off.width; Y2=height;
	
	Full_Reset();
	settings_off();
	game();
}
