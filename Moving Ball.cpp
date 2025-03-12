#include <graphics.h>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <chrono>
using namespace std;

const double PI=2*acos(0.0);

double rad(double deg){ // Degree to radians
	return deg*PI/180;
}

const int X=GetSystemMetrics(SM_CXSCREEN),Y=GetSystemMetrics(SM_CYSCREEN); // Height and Width of screen
const int ground=3*Y/4; // Height of ground from top of screen
const int boundary_color=BLACK;
const int background_speed=2; // Rate at which the background moves backwards

int page=0; // Active and visual page
int Time=0; // timer
int score=0; // Player's score

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int rand_num(int a, int b) {
    uniform_int_distribution<int> dist(a, b);
    return dist(rng);
}

class Ball{
	private:
		const int radius=50;
		int x=200,y=ground-radius-2;
		double vx=0;
		int vy=0;
		double angle=0.0;
	public:
		int getX(){return x;}
		int getY(){return y;}
		int getRadius(){return radius;}
		
		void print(){
			setcolor(boundary_color);
			
			circle(x,y,radius);
			setfillstyle(SOLID_FILL,12);
			floodfill(x,y,boundary_color);
			
			int bx=(double)radius*cos(rad(angle));
			int by=(double)radius*sin(rad(angle));
			line(x+bx,y-by,x-bx,y+by); // Rotating lines on ball
			line(x-by,y-bx,x+by,y+bx); // Rotating lines on ball
			
			// Rotating yellow ends of ball
			setcolor(14);
			setlinestyle(0,0,5);
			
			arc(x,y,angle-2.5,angle+2.5,radius);
			arc(x,y,angle+87.5,angle+92.5,radius);
			arc(x,y,angle+177.5,angle+182.5,radius);
			arc(x,y,angle+267.5,angle+272.5,radius);
			angle-=(vx>=0)? 5.0:-5.0;
		}
		void move(){
			y-=vy;
			x+=vx;
			if(y!=ground-radius-2) vy-=1;
			x=max(radius,x);
			x=min(X-radius,x);
			y=min(y,ground-radius-2);
			y=max(radius,y);
			if(vy<0 and y==ground*1.0-radius-2) vy/=-2;
		}
		void control(){
			if(GetAsyncKeyState(VK_UP));
			if(vy==0 and y==ground-radius-2 and GetAsyncKeyState(VK_UP)) vy=20;
			if(GetAsyncKeyState(VK_RIGHT)) vx=5;
			else if(GetAsyncKeyState(VK_LEFT)) vx=-5;
			else if(vy==0 and y==ground-radius-2) vx=0;
			if(vy || vx || y!=ground-radius-2) move();
		}
		
}ball;

struct Point{ // 2D coordinates
	int x,y;
};

class Coin{
	private:
		static const int number=10;  // # of coins in 2*screen range
		static const int colors=4;
		const int radius=20;
		const int cycle=2;           // Time (in sec) after which default coins change colour
		Point coin[number]; 	 	 // center of coins
		int color[number];       	 // color of coin[i]
		int COLOR[colors];       	 // color_id of color[j] w.r.t graphics.h
		int value[colors];  	 	 // value of color[j]. 1 is default value. -1 = Reset score to 0.
  		int probability[colors]; 	 // P(color[j])
  		pair<int,int> sound[colors]; // Sound when coin of color[j] is eaten
	public:
		void print(){
			for(int i=0;i<number;i++){
				int col=COLOR[color[i]];
				setcolor(col);
				circle(coin[i].x,coin[i].y,radius);
				setfillstyle(SOLID_FILL,col);
				floodfill(coin[i].x,coin[i].y,col);
			}
		}
		void newcoin(int i){
			bool present=true;
			int x=rand_num(1,100);
			for(int j=colors-1;j>=0;j--){
				if((x%probability[j])==0){
					color[i]=j;
					break;
				}
			}
			while(present){
				coin[i].x=X+100*rand_num(0,X/100);
				coin[i].y=250+100*rand_num(0,2);
				present=false;
				for(int j=0;j<number && !present;j++){
					if(i!=j && (abs(coin[i].x-coin[j].x)-1<=2*radius && coin[i].y==coin[j].y)){
						present=true;
					}
				}
			}
		}
		void move(){ // Move coins horizontally left
			for(int i=0;i<number;i++) coin[i].x-=background_speed; // Move coins;
			for(int i=0;i<number;i++){
				if(coin[i].x<=-radius){ // restore coins
					newcoin(i);
				}
				if(value[color[i]]==1) color[i]=(Time/cycle)&1;
			}
		}
		void eat(){
			int ball_x=ball.getX(),ball_y=ball.getY();
			int radius_ball=ball.getRadius();
			for(int i=0;i<number;i++){
				if(pow(coin[i].x-ball_x,2)+pow(coin[i].y-ball_y,2)<=(radius+radius_ball)*(radius+radius_ball)){
					switch(value[color[i]]){
						case -1:
							score=0;
							break;
						default:
							score+=value[color[i]];
							break;
					}
					Beep(sound[color[i]].first,sound[color[i]].second);
					newcoin(i); // Restore eaten coin
				}
			}
		}
		
		// Initialize coins
		Coin(){
			probability[0]=1;
			probability[1]=1;
			probability[2]=10;
			probability[3]=11; // If P[j]|rand(1,100) choose j, otherwise go up
			
			COLOR[0]=13; COLOR[1]=14; COLOR[2]=4; COLOR[3]=0;
			value[0]=value[1]=1; value[2]=5; value[3]=-1;
			sound[0]=sound[1]={700,100}; sound[2]={900,200}; sound[3]={400,400};

			for(int i=0;i<number;i++) newcoin(i); // Positions of coins
			for(int i=0;i<number;i++){ // color of coins
				int x=rand_num(1,100);
				for(int j=colors-1;j>=0;j--){
					if((x%probability[j])==0){
						color[i]=j;
						break;
					}
				}
			}
		}
}coin;


class Cloud{
	private:
		static const int number=3;  // # of clouds blocks in 2*screen range
		const int min_width=50,max_width=300;   // min. and max. width of clouds
		const int min_height=50,max_height=200; // min. and max. height of clouds
		const int min_dist=200,max_dist=500;    // min. and max. distance between 2 consecutive clouds
		Point top_left[number]; 	// Top left Point of each cloud
		Point bottom_right[number]; // Bottom right Point of each cloud
	public:
		void print(){
			setcolor(7);
			for(int i=0;i<number;i++) rectangle(top_left[i].x,top_left[i].y,bottom_right[i].x,bottom_right[i].y);
			setfillstyle(SOLID_FILL,WHITE);
			for(int i=0;i<number;i++){
				floodfill(top_left[i].x+3,(top_left[i].y+bottom_right[i].y)/2,7);
				floodfill(bottom_right[i].x-3,(top_left[i].y+bottom_right[i].y)/2,7);
			}
		}
		void move(){
			for(int i=0;i<number;i++){ // Move clouds
				top_left[i].x-=background_speed;
				bottom_right[i].x-=background_speed;
			}
			// Restore lost leftmost cloud
			if(bottom_right[0].x<=0){
				for(int i=1;i<number;i++){ // Shift remaining clouds to left
					top_left[i-1]=top_left[i];
					bottom_right[i-1]=bottom_right[i];
				}
				// Replace lost cloud by rightmost
				int i=number-1;
				top_left[i]={bottom_right[i-1].x+rand_num(min_dist,max_dist),rand_num(50,200)};
				bottom_right[i]={top_left[i].x+rand_num(min_width,max_width),top_left[i].y+rand_num(min_height,max_height)};
			}
		}
		// Initialize clouds
		Cloud(){
			top_left[0]={rand_num(100,500),rand_num(50,150)};
			bottom_right[0]={top_left[0].x+rand_num(50,250),top_left[0].y+rand_num(50,150)};
			
			for(int i=1;i<number;i++){
				top_left[i]={bottom_right[i-1].x+rand_num(min_dist,max_dist),rand_num(50,200)};
				bottom_right[i]={top_left[i].x+rand_num(min_width,max_width),top_left[i].y+rand_num(min_height,max_height)};
			}
		}
}cloud;

class Mountain{
	private:
		static const int width=300;
		static const int number=6; // number=X/mountain_width+2; # of mountains in screen range
		int height[number];
		int mountain[number]; // Location of mountains on X-axis
	public:
		void print(){
			setcolor(2);
			setfillstyle(SOLID_FILL,2);
			for(int i=0;i<number;i++){
				line(mountain[i],ground,mountain[i]+width,ground);
				line(mountain[i],ground,mountain[i]+width/2,ground-height[i]);
				line(mountain[i]+width,ground,mountain[i]+width/2,ground-height[i]);
				floodfill(mountain[i]+5,ground-1,2);
				floodfill(mountain[i]+width-5,ground-1,2);
			}
		}
		void move(){
			for(int i=0;i<number;i++) mountain[i]-=background_speed; // Move mountains
   			// Restore lost leftmost mountain
			if(mountain[0]<=-width-3){
				for(int i=1;i<number;i++){ // Shift remaining mountains to left
					mountain[i-1]=mountain[i];
					height[i-1]=height[i];
				}
				// Replace lost mountain by rightmost
				mountain[number-1]=mountain[number-2]+width+3;
				height[number-1]=rand_num(180,350);
			}
		}
		// Initialize mountains
		Mountain(){
			for(int i=0;i<number;i++) height[i]=rand_num(180,350);
			for(int i=0;i<number;i++) mountain[i]=3+300*i;
		}
}mountain;

void print(){
	setvisualpage(1-page); // Switches between current page being dispalyed
	setactivepage(page);   // New page to be displayed
	cleardevice();
	
	// Print ground
	setlinestyle(0,0,1);
	rectangle(0,ground,X,Y);
	line(0,ground,X,ground);
	setfillstyle(SOLID_FILL,7);
	floodfill(X-10,Y-10,boundary_color);
	
	// Printing mountains;
	mountain.print();
	
	// Printing ball
	ball.print();
	
	//sun
	setcolor(14);
	circle(200,100,50); 
	setfillstyle(SOLID_FILL,14);
	floodfill(200,100,14);
	
	
	//clouds
	cloud.print();
	
	//coins
	coin.print();
	
	// Printing Texts
	setcolor(boundary_color);
	char c[4];
	sprintf(c,"%d",Time);
	outtextxy(X/2,50,c);
	
	char sc[7]="Score:";
	outtextxy(X/2+200,50,sc);
	
	char sco[4];
	sprintf(sco,"%d",score);
	outtextxy(X/2+320,50,sco);
	page=1-page; // Switching active and visual page
}

void move_background(){
	// Move clouds
	cloud.move();
	
	// Move mountains
	mountain.move();
	
	// Move coins
	coin.move();
}

int main(){
	srand(time(NULL));
	initwindow(X,Y,"",-3,-3);
	setbkcolor(11);
	
	time_t ti,tf;
	time(&ti);
	settextstyle(4,0,3);
	
	while(!GetAsyncKeyState(VK_ESCAPE)){
		time(&tf);
		if(ti!=tf){
			Time++;
			ti=tf;
		}
		move_background();
		ball.control();
		coin.eat();
		print();
		delay(10);
	}
	closegraph();
	return 0;
}