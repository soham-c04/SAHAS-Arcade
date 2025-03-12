#include <graphics.h>
#include <unistd.h>

#include "Screen.h"
#include "2048.h"
#include "Air_Hockey.h"
#include "Snake.h"

int main(){
	initwindow(width,height,"",-3,-3);
	cleardevice();
	readimagefile("resources/opening_screen.jpg",0,0,width,height);
	sleep(5);
	cleardevice();
	int choice=0;
	page=0;
	while(1){
		setvisualpage(page);
		setactivepage(1-page);
		if(GetAsyncKeyState(VK_DOWN)){
			while(GetAsyncKeyState(VK_DOWN));
			if(++choice>3) choice=1;
		}
		
		else if(GetAsyncKeyState(VK_UP)){
			while(GetAsyncKeyState(VK_UP));
			if(--choice<1) choice=3;
		}
		
		else if(GetAsyncKeyState(VK_ESCAPE)) break;
		
		else if(GetAsyncKeyState(VK_RETURN)){
			switch(choice){
				case 1:
					cleardevice();
					page=1-page;
					setvisualpage(page);
					setactivepage(1-page);
					page=1-page;
					start_2048();
					break;
				case 2:
					cleardevice();
					page=1-page;
					setvisualpage(page);
					setactivepage(1-page);
					page=1-page;
					start_AH();
					break;
				case 3:
					cleardevice();
					page=1-page;
					setvisualpage(page);
					setactivepage(1-page);
					page=1-page;
					start_SG();
					break;
				default:
					break;
			}
		}
		switch(choice){
			case 1:
				readimagefile("resources/main_menu.jpg",0,0,width,height);
				readimagefile("resources/icon_2048.jpg",800,65,1000,265);
				readimagefile("resources/icon_Air_Hockey.jpg",825,285,975,435);
				readimagefile("resources/icon_Snake_Game.jpg",825,480,975,630);
				break;
			case 2:
				readimagefile("resources/main_menu.jpg",0,0,width,height);
				readimagefile("resources/icon_2048.jpg",825,90,975,240);
				readimagefile("resources/icon_Air_Hockey.jpg",800,260,1000,460);
				readimagefile("resources/icon_Snake_Game.jpg",825,480,975,630);
				break;
			case 3:
				readimagefile("resources/main_menu.jpg",0,0,width,height);
				readimagefile("resources/icon_2048.jpg",825,90,975,240);
				readimagefile("resources/icon_Air_Hockey.jpg",825,285,975,435);
				readimagefile("resources/icon_Snake_Game.jpg",800,455,1000,655);
				break;
			default:
				readimagefile("resources/main_menu.jpg",0,0,width,height);
				readimagefile("resources/icon_2048.jpg",825,90,975,240);
				readimagefile("resources/icon_Air_Hockey.jpg",825,285,975,435);
				readimagefile("resources/icon_Snake_Game.jpg",825,480,975,630);
				break;
		}
		page=1-page;
		delay(250);
	}
	getch();
	closegraph();
	return 0;
}
