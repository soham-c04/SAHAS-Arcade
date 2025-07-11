#include "Snake.h"
#include "Screen.h"
using namespace std;

int d=1;//direction of snake
int dir=1;//dynamic map
int rx=0,ry=0;
int f=1;//counter
char lengtharr[3];
int end_t=0;
int endsnake(int e,int a);

void start_SG(){
	cleardevice();
	rx=(width/2)-((width/2)%10),ry=(height/2)-((height/2)%10);
	end_t=0;dir=1;d=1;
	int x[200],y[200],chk=700,length=1;

	for (int i=0;i<200;i++)
	{
		x[i]=0;
		y[i]=0;
	}
	
	int k=0;
	if (length>=5 && length<10)
		 k=1;
	else if (length>=10 && length<15)
	 	k=2;
	else if (length>=15 && length<20)
		 k=3;
	else if (length>=20) 
		k=4;
	delay(5-k);
	srand(time(NULL));
	setfillstyle(1,2);
	x[0]=(width/2)-((width/2)%10);
	y[0]=(height/2)-((height/2)%10);
	d=1;
	settextstyle(3, HORIZ_DIR, 7);
	for(;;)
	{
		setactivepage(page);
        setvisualpage(1-page);
		setfillstyle(1,10);//set screen light green
		bar(10,210,width-10,height-10);
		setfillstyle(1,2);//boundary of game set green
		bar(0,200,width,210);
		bar(0,200,10,height);
		bar(0,height,width,height-10);
		bar(width-10,210,width,height);
		
		//making food appear after eating by snake
		if(x[0]==rx && y[0]==ry)
		{
			f++;
			length+=1;
			setfillstyle(1,4);
			bar(rx,ry,rx+10,ry+10);
			do
			{
				rx=20+rand()%(width-40);
				ry=210+rand()%(height-220);
			}
			while(getpixel(rx,ry)!=10  && rx>10 && ry>10);//to spawn food inside the boundary
			//convert to integer
			rx/=10;
			rx*=10;
			ry/=10;
			ry*=10;
		}
		setfillstyle(1,4); //foodcolor
		bar(rx,ry,rx+10,ry+10);
		
		//making food appear initially
		
		
		if(GetAsyncKeyState(VK_RIGHT))
		{
			d=1;
		}
		else if(GetAsyncKeyState(VK_LEFT))
		{
			d=2;
		}
		else if(GetAsyncKeyState(VK_UP))
		{
			d=3;
		}
		else if(GetAsyncKeyState(VK_DOWN))
		{
			d=4;
		}
		else if(GetAsyncKeyState(VK_ESCAPE))
		{
			d=5;
		}
		else
		{
			d=0;
		}
		switch(d)
		{
			case 0:
				if(dir==1)
				{
					x[0]+=10;
				}
				else if(dir==2)
				{
					x[0]-=10;
				}
				else if(dir==3)
				{
					y[0]-=10;
				}
				else if(dir==4)
				{
					y[0]+=10;																																																				
				}
				else
				{
					d=0;
				}
				break;
				
			case 1:
				x[0]+=10;
				dir=1;
				break;
			case 2:
				x[0]-=10;
				dir=2;
				break;
				
			case 3:
				y[0]-=10;
				dir=3;
				break;
				
			case 4:
				y[0]+=10;
				dir=4;
				break;
			case 5:
				end_t+=1;
				break;
		}
		setfillstyle(1,9);//snakebodycolor
		for (int i=1;i<=length;i++)
		{
			bar(x[i],y[i],x[i]+10,y[i]+10);		
		}
		setfillstyle(1,1);//snakeheadcolor
		bar(x[0],y[0],x[0]+10,y[0]+10);
		for (int i=199;i>0;i--){
			x[i]=x[i-1];
			y[i]=y[i-1];
		}
		delay(100);
		for (int i=2;i<=length;i++){
			if(x[0]==x[i] && y[0]==y[i]){
				chk=i;
			}
		}
		if(x[0]==x[chk] && y[0]==y[chk])
		{
			endsnake(f,0);
			break;
		}		
		if(x[0]>=width-10 || x[0]<=10 || y[0]<=210 || y[0]>=height-10)
		{
			endsnake(f,1);
			break;
		}
		if(end_t==1)
		{
			endsnake(f,2);
			break;
		}
                 setcolor(RED);
                 char ch1[20]="LENGTH ACHIEVED:";
                 outtextxy(300,20,ch1);
                 lengtharr[0]=char(48+length/10);
                 lengtharr[1]=char(48+length%10);
                 lengtharr[2]='\0';                 
                 outtextxy(1070,20, lengtharr);
                 
    page = 1-page;
	}
}
int endsnake(int e,int a)
{
	setcolor(RED);
	e=e-1;
	bar(0,200,width,height);
	delay(2000);
	page = 1-page;
	setactivepage(page);
    setvisualpage(1-page);
	cleardevice();
	char ch1[11]="GAME OVER!";
	outtextxy(width/2-200,height/2-100,ch1);
	if(a==1)
	{
		char ch2[30]="You crossed the boundary!";
		outtextxy(width/2-400,height/2,ch2);
	}
	else if (a==0)
	{
		char ch2[30]="You bumped into yourself!";
		outtextxy(width/2-400,height/2,ch2);		
	}
	else if (a==2)
	{
		char ch2[30]="You pressed esc key!";
		outtextxy(width/2-400,height/2,ch2);	
	}
	page = 1-page;
	setactivepage(page);
    setvisualpage(1-page);
    delay(3000);
	getch();
	return 0;
}
