#include "Extensions/Screen.cpp"
#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>
#include <thread>
using namespace std;

#define F first
#define S second
#define DELAY 15

const int MAX_LEN = 1e3;
SOCKET serverSocket;
map<SOCKET,int> opponent;

// --- online START ---

struct Components;

void SendData(SOCKET sock, string message){
	message += '$';
	send(sock, message.c_str(), message.size(), 0);
}

void sendData(Components &comp);
vector<int> requestData(SOCKET client, vector<string> req){
	string message = "req ";
	for(string &s: req)
		message += s + ' ';
	SendData(client, message);
	
	char receive[MAX_LEN] = {0};
	vector<int> Data;
	if(recv(client, receive, MAX_LEN, 0) > 0){
		string data;
		int n = strlen(receive);
			
    	for(int i=0;i<n;i++){
    		if(receive[i] == ' '){
				try{
    				Data.push_back(stoi(data));
				}
				catch(const exception& e){
					cout << receive << "receive" << endl;
					cout << data << "Data" << endl;
				}
    			data.clear();
			}
    		else
				data.push_back(receive[i]);
		}
	}
	return Data;
}

int check_collision(Components &comp);

struct Striker{
	const int radius=50;		
	const int speed=10;			
	int x,y; 	  				
	int color; 	  				
	SOCKET client;                  // SOCKET for corr. client of Striker

	void reset(int who){			
		if(who==1) x=X1+radius+10;	
		else x=X2-radius-10;        
		y=(Y2+Y1)/2; 				
	}

	void control(int who, Components &comp){
		vector<int> receive;
		if(who == 1)
			receive = requestData(client, {"W","A","S","D"});
		else
			receive = requestData(client, {"VK_UP", "VK_LEFT", "VK_DOWN", "VK_RIGHT"});

		int up=receive[0], left=receive[1], down=receive[2], right=receive[3];
		int incx=0,incy=0;
		if(up) incy=speed;
		if(down) incy=-speed;
		if(left) incx=-speed;
		if(right) incx=speed;

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

		if(check_collision(comp)>=1){
			x-=incx;
			y+=incy;
		}
	}
};

class Ball{
	private:
		const int start_speed=20; 
	public:
		const int radius=25;
		int speed=20;   	 
		int x,y; 			 
		double angle=90.0; 	 

		void reset(){ 
			speed=start_speed; 
			x=(X2+X1)/2;  
			y=(Y2+Y1)/2; 
			angle=90.0;  
		}

		void move(Components &comp, bool Goaling=false);  
		void boundary(Components &comp);
		
		void collision(Components &comp, Striker &s){
			if(pow(s.x-x,2)+pow(s.y-y,2)<=pow(s.radius+radius,2)){
				double theta=deg(atan2(s.y-y,x-s.x));

				double anglee=angle;
				angle=fmod(fmod(180.0+2*theta-angle,360.0)+360.0,360.0);

				int incx=round(speed*cos(rad(anglee))), incy=round(speed*sin(rad(anglee)));
				if(fabs(anglee-angle)>10.0){
					x-=incx;
					y+=incy;
				}
				else move(comp); //Trying to debug tangential collisions with striker moving towards them
			}
		}
};

struct Goal{
	int width, height;
	int goal_1=0, goal_2=0;  // goal counter for first and second player respectively

	void goaling(int who, Components &comp);
};

struct Components{  // Easier to pass by reference
	int Timer;
	Ball ball;
	Striker striker1, striker2;
	Goal goal;

	void reset(){
		ball.reset();
		striker1.reset(1);
		striker2.reset(2);
	}

	void Full_Reset(){
		reset();
		goal.goal_1=0;
		goal.goal_2=0;
		Timer = 90;
	}
	
	void sendData(){
		string message;
		message += to_string(Timer) + ' ';
		
		message += to_string(ball.x) + ' ';
		message += to_string(ball.y) + ' ';
		
		message += to_string(striker1.x) + ' ';
		message += to_string(striker1.y) + ' ';
		
		message += to_string(striker2.x) + ' ';
		message += to_string(striker2.y) + ' ';
		
		message += to_string(goal.goal_1) + ' ';
		message += to_string(goal.goal_2) + ' ';

		SendData(striker1.client, message);
		SendData(striker2.client, message);
	}
};

int check_collision(Components &comp){ // Function to check collision of ball and strikers
	int ans=0;
	// Collision of ball with first striker
	if(pow(comp.striker1.x-comp.ball.x,2)+pow(comp.striker1.y-comp.ball.y,2)<pow(comp.striker1.radius+comp.ball.radius,2)) ans++;
	// Collision of ball with second striker
	if(pow(comp.striker2.x-comp.ball.x,2)+pow(comp.striker2.y-comp.ball.y,2)<pow(comp.striker1.radius+comp.ball.radius,2)) ans+=2;
	// Collision of one striker with another
	if(pow(comp.striker1.x-comp.striker2.x,2)+pow(comp.striker1.y-comp.striker2.y,2)<=pow(comp.striker1.radius+comp.striker2.radius,2)) ans+=3;

	return ans;
}

// ---- Ball ----

void Ball::move(Components &comp, bool Goaling){
	int incx=round(speed*cos(rad(angle))); // X component of speed of ball
	int incy=round(speed*sin(rad(angle))); // Y component of speed of ball
	x+=incx; // increment in X coordinate of ball
	y-=incy; // increment in Y coordinate of ball
	if(!Goaling){
		if(y<((Y2+Y1)/2-comp.goal.height+radius) || y>((Y2+Y1)/2+comp.goal.height-radius)){
			x=max(x,X1+radius);
			x=min(x,X2-radius);
		}
		y=max(y,Y1+radius);
		y=min(y,Y2-radius);

		boundary(comp);
	}
}

void Ball::boundary(Components &comp){
	if(x<=(radius+X1) && y>=((Y2+Y1)/2-comp.goal.height+radius) && y<=((Y2+Y1)/2+comp.goal.height-radius)){
		comp.goal.goaling(1, comp); // Ball in left goal
		return;
	}
	else if((x+radius)>=X2 && y>=((Y2+Y1)/2-comp.goal.height+radius) && y<=((Y2+Y1)/2+comp.goal.height-radius)){
		comp.goal.goaling(2, comp); // Ball in right goal
		return;
	}
	// Collision with left,right and top,down boundaries
	if(x<=(radius+X1) || (x+radius)>=X2) angle=fmod(fmod(180.0-angle,360.0)+360.0,360.0);
	if(y<=(radius+Y1) || (y+radius)>=Y2) angle=fmod(fmod(360.0-angle,360.0)+360.0,360.0);
}

// ---- Goal ----

void Goal::goaling(int who, Components &comp){ //checks goal condition and updates value if goal is scored
	comp.ball.speed=3; // Slow down for goal animation
	if(who==1){ // gone into player 1's net
		goal_2++; // Player 2 scores
		while(comp.ball.x>(X1-3*width/4) && comp.ball.y>=((Y2+Y1)/2-height+comp.ball.radius) && comp.ball.y<=((Y2+Y1)/2+height-comp.ball.radius)){ // Continue animation till be ball reaches any edge of the net
			comp.sendData();
			comp.ball.move(comp, true);
			delay(DELAY);
		}
	}
	else{
		goal_1++; //player 1 scores
		while(comp.ball.x<(X2+3*width/4) && comp.ball.y>=((Y2+Y1)/2-height+comp.ball.radius) && comp.ball.y<=((Y2+Y1)/2+height-comp.ball.radius)){
			comp.sendData();
			comp.ball.move(comp, true);
			delay(DELAY);
		}
	}

	SendData(comp.striker1.client, "Goal ");
	SendData(comp.striker2.client, "Goal ");
	
	comp.reset();
	
	sleep(2);
}

void start(Components &comp){
	time_t ti,tf;
	time(&ti);

	comp.sendData();
	while(comp.Timer >= 0){
		time(&tf);
		if(ti!=tf){ //incrementing time
			ti=tf;
			comp.Timer--;
		}

		// Physics
		comp.ball.move(comp);
		comp.ball.collision(comp, comp.striker1);
		comp.ball.collision(comp, comp.striker2);
		comp.striker1.control(1, comp);
		comp.striker2.control(2, comp);
		
		comp.sendData();

		delay(DELAY);
	}
	
	if(comp.goal.goal_1 > comp.goal.goal_2){
		SendData(comp.striker1.client, "Win ");
		SendData(comp.striker2.client, "Lose ");
	}
	else if(comp.goal.goal_1 < comp.goal.goal_2){
		SendData(comp.striker1.client, "Lose ");
		SendData(comp.striker2.client, "Win ");
	}
	else{
		SendData(comp.striker1.client, "Draw ");
		SendData(comp.striker2.client, "Draw ");
	}
}

void online(Components &comp){
	comp.goal.width = 90*Xratio;
	comp.goal.height= 125*Yratio;

	X1=X+comp.goal.width; 	 Y1=Y;
	X2=width-comp.goal.width; Y2=height;

	comp.Full_Reset();
	
	start(comp);
}

// ---- online END ----

void Clients(SOCKET clientSocket){
    char receive[MAX_LEN];
    while(opponent[clientSocket] == -1){
    	for(auto oppo: opponent){
			if(oppo.F != clientSocket && oppo.S == -1){
				opponent[clientSocket] = oppo.F;
				opponent[oppo.F] = clientSocket;
			}
		}
	}
	
	int opp = opponent[clientSocket];
	
	Components comp;
	if(clientSocket < opp){
		requestData(clientSocket, {"done?"});
		requestData(opp, {"done?"});
		
		comp.striker1.client = clientSocket;
		comp.striker2.client = opp;
		online(comp);
		
		opponent.erase(clientSocket);
		opponent.erase(opp);

		closesocket(clientSocket);
		closesocket(opp);
	}
	
//	else{
//		while(opponent[clientSocket] != -1){
//			sleep(1);
//		}
//	}
//
//    while(true){
//    	memset(receive, 0, MAX_LEN);
//
//	    if(recv(clientSocket, receive, MAX_LEN, 0) > 0){
//	    	if(strcmp(receive, "-4 ") == 0){
//	    		cout << endl << "Client:" << clientSocket << " disconnected" << endl;
//	    		opponent.erase(clientSocket);
//				break;
//			}
//			else if(strcmp(receive, "-3 ") == 0){
//	    		cout << endl << "Client: " << clientSocket << " unpaired from Client: " << opp << endl;
//	    		opponent[clientSocket] = opponent[opp] = -1;
//			}
//			else if(strcmp(receive, "-2 ") == 0){
//				while(opponent[clientSocket] == -1){
//			    	for(auto oppo: opponent){
//						if(oppo.F != clientSocket && oppo.S == -1){
//							opponent[clientSocket] = oppo.F;
//							opponent[oppo.F] = clientSocket;
//						}
//					}
//				}
//				
//				opp = opponent[clientSocket];
//				
//				if(clientSocket < opp){
//					requestData(clientSocket, {"done?"});
//					requestData(opp, {"done?"});
//
//					comp.striker1.client = clientSocket;
//					comp.striker2.client = opp;
//					online(comp);
//				}
//				else{
//					while(opponent[clientSocket] != -1){
//						sleep(1);
//					}
//				}
//			}
//		}
//	}
//
//	closesocket(clientSocket);
}

int startServer(){
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        cout << "WSAStartup failed\n";
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == INVALID_SOCKET){
        cout << "ERROR opening socket\n";
        WSACleanup();
        return 2;
    }

	int port_no;
	cout<<"Enter the port no. for server: ";
	cin>>port_no;

	struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port_no);

    if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){
        cout << "ERROR on binding\n";
        closesocket(serverSocket);
        WSACleanup();
        return 3;
    }

    return 0;
}

int main(){
	srand(time(0));
	int err;
	if(err = startServer())
		return err;
	
	auto start_time=chrono::high_resolution_clock::now();
	auto end_time=chrono::high_resolution_clock::now();
	int timeout = 120; // 120 sec timeout for server
	
	while(chrono::duration_cast<chrono::seconds>(end_time - start_time).count() < timeout){
		end_time=chrono::high_resolution_clock::now();
		if(listen(serverSocket, 5) == SOCKET_ERROR){
	        cout << "ERROR on listen\n";
	        closesocket(serverSocket);
	        WSACleanup();
	        return 4;
	    }

		struct sockaddr_in clientAddr;
		int clilen = sizeof(clientAddr);
	    SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clilen);
	    if (clientSocket == INVALID_SOCKET){
	        cout << "ERROR on accept\n";
	        continue;
		}
	    
	    if(opponent.find(clientSocket) == opponent.end()){
	    	start_time=chrono::high_resolution_clock::now();    // Restart timer
			opponent[clientSocket] = -1;
			cout << endl << "Client connected!\nSocket: " << clientSocket << endl;
	        thread t(Clients, clientSocket);
	        t.detach();
		}
	}
	
	cout << endl << "Server closed due to Timeout: " << timeout << "seconds" << endl;

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

