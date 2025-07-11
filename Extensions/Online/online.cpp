#include "../Screen.h"
#include "online.h"
#include "client.h"
#include <unistd.h>
#include <thread>

#define F first
#define S second

int Timer;  				// Time counter
int result; 				// Who won the game

void print();

struct Ball{
	const int radius=25;	// Radius of ball
	int x,y; 			 	// Coords of ball
}ball;

struct Striker{
	const int radius=50;	// Radius of striker
	int x,y; 	  			// Coords of strikers
	int color; 	  			// Color of strikers
}striker1, striker2;

struct Goal{
	int width, height;
	int goal_1=0, goal_2=0;
}goal;

void text(){//prints texts while game is running
	char g1[3]="00";
	char g2[3]="00";
	char c1[]="TIME LEFT",c2[]="PLAYER 1",c3[]="PLAYER 2",c4[]="Press Escape",c5[]="for Menu",c6[]="Press Escape",c7[]="for Menu";
		g1[1]=(char)(goal.goal_1%10+'0');
		g1[0]=(char)(goal.goal_1/10+'0');
		g2[1]=(char)(goal.goal_2%10+'0');
		g2[0]=(char)(goal.goal_2/10+'0');
	outtextxy((X1+X2)/2-108*Xratio-40*Xratio,30*Yratio,g1);
	outtextxy((X1+X2)/2+70*Xratio+45*Xratio,30*Yratio,g2);
	char t[4];
	sprintf(t,"%d",Timer);
	int tme=Timer,sz=0;
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

void print(){
	setactivepage(page);
	setvisualpage(1-page);
	print_field();
	color();
	text();
	print_movable();
	page=1-page;
}

void settings(){
	setactivepage(page);
 	setvisualpage(page);
	cleardevice();
	setcolor(1);
	setlinestyle(0,0,5);
	rectangle(50,50,width-50,height-50);
	setfillstyle(SOLID_FILL,8);
	floodfill(width/2,height/2,1);
	char c1[]="Choose color for PLAYER 1:";
	settextstyle(3,HORIZ_DIR,7);
	outtextxy(width/4-100,100,c1); //prints text for player 1 to choose color
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
		delay(DELAY);
	}

	while(GetAsyncKeyState('1') || GetAsyncKeyState('2')) delay(DELAY); //to avoid misckicking of keys

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
		delay(DELAY);
	}
	while(GetAsyncKeyState('1') || GetAsyncKeyState('2')) delay(DELAY); //to avoid misckicking of keys

	settextstyle(4,HORIZ_DIR,3*Xratio);

	sendData({1});
}

void find_match(){
	cleardevice();
	char connected[] = "Connected to server!";
	settextstyle(3,HORIZ_DIR,5);
	outtextxy((X1+X2)/2-350*Xratio, (Y1+Y2)/2 - 200*Yratio, connected);
	settextstyle(3,HORIZ_DIR,7);
	char waiting[] = "Waiting for opponent...";
	outtextxy((X1+X2)/2-350*Xratio, (Y1+Y2)/2 - 50*Yratio, waiting);
	settextstyle(4,HORIZ_DIR,3*Xratio);

	if(receiveData().S[0] == "done?")
		cout << "Working" << endl;
}

void server_details(){
	setvisualpage(page);
	setactivepage(page);

	char ip[] = "Server ip: ";
	char port[] = "Port no:";
	char retry[] = "Unable to connect. Please retry";
	settextstyle(3,HORIZ_DIR,6);

	string server_ip;
	int port_no=-1;
	do{
		cleardevice();
		outtextxy((X1+X2)/2-350*Xratio, (Y1+Y2)/2 - 150*Yratio, ip);

		if(port_no != -1)
			outtextxy((X1+X2)/2-350*Xratio, (Y1+Y2)/2 + 150*Yratio, retry);

		int len = 0;
		char serv[21] = {0};
		while(!GetAsyncKeyState(VK_RETURN)){
			if(len <= 20){
				for(char c='0';c<='9';c++){
					if(GetAsyncKeyState(c)){
						serv[len++] = c;
						outtextxy((X1+X2)/2 - 90*Xratio, (Y1+Y2)/2 - 150*Yratio, serv);
					}
				}
				for(char c='A';c<='Z';c++){
					if(GetAsyncKeyState(c)){
						serv[len++] = c|32;
						outtextxy((X1+X2)/2 - 90*Xratio, (Y1+Y2)/2 - 150*Yratio, serv);
					}
				}
				if(GetAsyncKeyState(VK_OEM_PERIOD)){
					serv[len++] = '.';
					outtextxy((X1+X2)/2 - 90*Xratio, (Y1+Y2)/2 - 150*Yratio, serv);
				}
			}
			if(GetAsyncKeyState(VK_BACK)){
				cleardevice();
				port_no = -1;
				outtextxy((X1+X2)/2-350*Xratio, (Y1+Y2)/2 - 150*Yratio, ip);

				len=max(0,len-1);
				serv[len] = 0;
				outtextxy((X1+X2)/2 - 90*Xratio, (Y1+Y2)/2 - 150*Yratio, serv);
			}

			delay(DELAY*15);
		}
		while(GetAsyncKeyState(VK_RETURN)) delay(DELAY*10);

		outtextxy((X1+X2)/2-350*Xratio, (Y1+Y2)/2, port);

		len = 0;
		char portno[11] = {0};

		while(!GetAsyncKeyState(VK_RETURN)){
			if(len <= 10){
				for(char c='0';c<='9';c++){
					if(GetAsyncKeyState(c)){
						portno[len++] = c;
						outtextxy((X1+X2)/2 - 90*Xratio, (Y1+Y2)/2, portno);
					}
				}
			}
			if(GetAsyncKeyState(VK_BACK)){
				cleardevice();
				port_no = -1;
				outtextxy((X1+X2)/2-350*Xratio, (Y1+Y2)/2 - 150*Yratio, ip);
				outtextxy((X1+X2)/2-350*Xratio, (Y1+Y2)/2, port);

				len=max(0,len-1);
				portno[len] = 0;
				outtextxy((X1+X2)/2 - 90*Xratio, (Y1+Y2)/2 - 150*Yratio, serv);
				outtextxy((X1+X2)/2 - 90*Xratio, (Y1+Y2)/2, portno);
			}

			delay(DELAY*15);
		}
		while(GetAsyncKeyState(VK_RETURN)) delay(DELAY*10);

		server_ip = serv;
		try{
			port_no = stoi(portno);
		}
		catch(const exception& e){
			port_no = -2;
		}

		memset(serv, 0, 21);
		memset(portno, 0, 11);
	}
	while(connect_to_server(server_ip,port_no));
}

bool goaling = false;
void update_components(){
	pair<vector<int>, vector<string>> receive;
	while(result == -1){
		receive = receiveData();
		
		vector<int> send_;
		for(string &req: receive.S){
			if(req == "W")
				send_.push_back(GetAsyncKeyState('W') != 0);
			else if(req == "A")
				send_.push_back(GetAsyncKeyState('A') != 0);
			else if(req == "S")
				send_.push_back(GetAsyncKeyState('S') != 0);
			else if(req == "D")
				send_.push_back(GetAsyncKeyState('D') != 0);
			else if(req == "VK_UP")
				send_.push_back(GetAsyncKeyState(VK_UP) != 0);
			else if(req == "VK_LEFT")
				send_.push_back(GetAsyncKeyState(VK_LEFT) != 0);
			else if(req == "VK_DOWN")
				send_.push_back(GetAsyncKeyState(VK_DOWN) != 0);
			else if(req == "VK_RIGHT")
				send_.push_back(GetAsyncKeyState(VK_RIGHT) != 0);
			else if(req == "Goal")
				goaling = true;
			else if(req == "Win")
				result = 1;
			else if(req == "Lose")
				result = 2;
			else if(req == "Draw")
				result = 0;
		}

		if(!send_.empty())
			sendData(send_);

		if(receive.F.empty())
			continue;

		int i=0;
		Timer = receive.F[i++];

		ball.x = receive.F[i++];
		ball.y = receive.F[i++];

		striker1.x = receive.F[i++];
		striker1.y = receive.F[i++];
		
		striker2.x = receive.F[i++];
		striker2.y = receive.F[i++];

		goal.goal_1 = receive.F[i++];
		goal.goal_2 = receive.F[i++];
	}
}

void start(){
	result = -1;
	
	thread t(update_components);
	t.detach();
	while(result == -1){
		if(goaling){
			setvisualpage(1-page);
			readimagefile("resources/Air_Hockey/goal.jpg",(X1+X2)/2-150*Xratio,(Y1+Y2)/2-75*Yratio,(X1+X2)/2+150*Xratio,(Y1+Y2)/2+75*Yratio);
			sleep(2);
			goaling = false;
		}
		else
			print();

//		delay(DELAY);
	}

	setvisualpage(0);
	setactivepage(0);
	if(result == 1){
		readimagefile("resources/Air_Hockey/you_win.jpg",(X1+X2)/2-300*Xratio,(Y1+Y2)/2-125*Yratio,(X1+X2)/2+300*Xratio,(Y1+Y2)/2+100*Yratio);
	}
	else if(result == 2){
		readimagefile("resources/Air_Hockey/you_lose.jpg",(X1+X2)/2-300*Xratio,(Y1+Y2)/2-125*Yratio,(X1+X2)/2+300*Xratio,(Y1+Y2)/2+100*Yratio);
	}
	else{
		readimagefile("resources/Air_Hockey/draw.jpg",(X1+X2)/2-250*Xratio,(Y1+Y2)/2-150*Yratio,(X1+X2)/2+350*Xratio,(Y1+Y2)/2+150*Yratio);
	}
	sleep(4);
}

void main_online(){
	goal.width = 90*Xratio;
	goal.height= 125*Yratio;

	X1=X+goal.width; 	 Y1=Y;
	X2=width-goal.width; Y2=height;

	server_details();
	find_match();
	settings();
	start();
	closeConnection();
}
