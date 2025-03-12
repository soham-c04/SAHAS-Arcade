#include <graphics.h>
#include <time.h>
#include "Screen.h"
#include "2048.h"
using namespace std;

int change=0,score=0,n=4;
int win_continue=0;

int shift_2048(int ar[4][4], int i, int j, int l)
{
    for(int k=j; k<(l-1); k++)
    {
        if (ar[i][k+1]!=0)
        {
            if(ar[i][k]==ar[i][k+1])
			{
                score+=2*ar[i][k];
                ar[i][k+1]=2*ar[i][k];//adds same number
                ar[i][k]=0;//makes left element 0
                change++;
            }
            return k+1;
        }
        else
        {
            ar[i][k+1]=ar[i][k];//shifting the selected element to the right
            ar[i][k]=0;//making previous element 0
            change++;
        }
    }
    return l;
}
int play_2048(int ar[4][4], int i, int j)
{ 
    static int k=4;
    if(i==n)
	{
        return 0;
    }
	else if(j==-1)
	{
		k=4;
        play_2048(ar, i+1, n-2);
    }
	else if(ar[i][j]==0)
	{
        play_2048(ar, i, j-1);
    }
	else
	{
        k = shift_2048(ar, i, j, k);
        play_2048(ar, i, j-1);
    }
}
int caller_2048(int ar[4][4])
{
	while(1)
	{
	    int temp[4][4];
	    if (GetAsyncKeyState(VK_UP)||GetAsyncKeyState('W'))
		{
			while(GetAsyncKeyState(VK_UP)||GetAsyncKeyState('W'));
	        for(int i=0; i<4; i++)
			{
	            for(int j=0; j<4; j++)
				{
	                temp[i][j]=ar[i][4-1-j];
	            }
	        }
	        play_2048(temp,0,4-2);
	        for(int i=0; i<4; i++)
			{
	            for(int j=0; j<4; j++)
				{
	                ar[i][j]=temp[i][4-1-j];
	            }
	        }
	        return 1;
	    }
		else if(GetAsyncKeyState(VK_LEFT)||GetAsyncKeyState('A'))
		{
			while(GetAsyncKeyState(VK_LEFT)||GetAsyncKeyState('A'));
	        for(int i=0; i<4; i++)
			{
	            for(int j=0; j<4; j++)
				{
	                temp[i][j]=ar[4-1-j][i];
	            }
	        }
	        play_2048(temp,0,4-2);
	        for(int i=0; i<4; i++)
			{
	            for(int j=0; j<4; j++)
				{
	                ar[i][j]=temp[j][4-1-i];
	            }
	        }
	        return 1;
	    }
		else if(GetAsyncKeyState(VK_RIGHT)||GetAsyncKeyState('D'))
		{
			while(GetAsyncKeyState(VK_RIGHT)||GetAsyncKeyState('D'));
	        for(int i=0; i<4; i++)
			{
	            for(int j=0; j<4; j++)
				{
	                temp[i][j]=ar[j][4-1-i];
	            }
	        }
	        play_2048(temp,0,4-2);
	        for(int i=0; i<4; i++)
			{
	            for(int j=0; j<4; j++)
				{
	                ar[i][j]=temp[4-1-j][i];
	            }
	        }
	        return 1;
	    }
		else if(GetAsyncKeyState(VK_DOWN)||GetAsyncKeyState('S'))
		{
			while(GetAsyncKeyState(VK_DOWN)||GetAsyncKeyState('S'));
	        play_2048(ar,0,4-2);
	        return 1;
		}
		else if(GetAsyncKeyState(VK_ESCAPE))
		{
			while(GetAsyncKeyState(VK_ESCAPE));
            return 0;
        }
    }
}
int win_check_2048(int ar[4][4])
{
    for (int i=0; i<4; i++)
	{
        for(int j=0; j<4; j++)
		{
            if(ar[i][j]==2048)
			{
				win_continue=1;
                return 1;
            }
        }
    }
    return 0;
}
int generator_2048(int ar[4][4])
{
    int count=0;
    for (int i=0; i<4; i++)
	{
        for(int j=0; j<4; j++)
		{
            if(ar[i][j]==0)
			{
                count++;
            }
        }
    }
    if(count!=0)
	{
        static int k=0;
        srand(k);
        int a=(rand())%4, b=(rand())%4;
        if(ar[a][b]!=0)
		{
            k++;
            generator_2048(ar);
        }
		else
		{
            ar[a][b]=2;
            k++;
        }
    }
	else
	{
        return 0;
    }
}
void grid_2048(int grid_ref_x[5],int grid_ref_y[5])
{
	rectangle(grid_ref_x[0],grid_ref_y[0],grid_ref_x[4],grid_ref_y[4]);
	line(grid_ref_x[1],grid_ref_y[0],grid_ref_x[1],grid_ref_y[4]);
	line(grid_ref_x[2],grid_ref_y[0],grid_ref_x[2],grid_ref_y[4]);
	line(grid_ref_x[3],grid_ref_y[0],grid_ref_x[3],grid_ref_y[4]);
	line(grid_ref_x[0],grid_ref_y[1],grid_ref_x[4],grid_ref_y[1]);
	line(grid_ref_x[0],grid_ref_y[2],grid_ref_x[4],grid_ref_y[2]);
	line(grid_ref_x[0],grid_ref_y[3],grid_ref_x[4],grid_ref_y[3]);
}	
void print_score_2048()
{
	char ch1[8]="Score: ";
	char ch2[16]="Press Escape to";
	char ch3[15]="Toggle Options";
	outtextxy(850*Xratio,200*Yratio,ch1);
	outtextxy(850*Xratio,450*Yratio,ch2);
	outtextxy(850*Xratio,520*Yratio,ch3);
	int score_board_ref[8]={850*Xratio,921*Xratio,992*Xratio,1064*Xratio,1135*Xratio,1207*Xratio,1278*Xratio,1350*Xratio};
	int length=1,temp=score;
	while(temp!=0)
	{
		length++;
		temp/=10;
	}
	length--;
	readimagefile("resources/2048/score_board.jpg",850*Xratio,265*Yratio,1350*Xratio,355*Yratio);
	temp=score;
	int digit;
	while(length!=0)
	{
		digit=temp%10;
		temp/=10;
		switch(digit)
		{
			case 0:
				readimagefile("resources/2048/digit_0.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			case 1:
				readimagefile("resources/2048/digit_1.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			case 2:
				readimagefile("resources/2048/digit_2.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			case 3:
				readimagefile("resources/2048/digit_3.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			case 4:
				readimagefile("resources/2048/digit_4.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			case 5:
				readimagefile("resources/2048/digit_5.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			case 6:
				readimagefile("resources/2048/digit_6.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			case 7:
				readimagefile("resources/2048/digit_7.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			case 8:
				readimagefile("resources/2048/digit_8.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			case 9:
				readimagefile("resources/2048/digit_9.jpg",(score_board_ref[length-1]+5),270*Yratio,(score_board_ref[length]-5),350*Yratio);
				break;
			default:
				break;
		}
		length--;
	}
}
void display_2048(int ar[4][4])
{
	cleardevice();
	int grid_ref_x[5]={400*Xratio,500*Xratio,600*Xratio,700*Xratio,800*Xratio};
	int grid_ref_y[5]={200*Yratio,300*Yratio,400*Yratio,500*Yratio,600*Yratio};
	grid_2048(grid_ref_x,grid_ref_y);
	print_score_2048();
	readimagefile("resources/2048/logo.jpg",50*Xratio,150*Yratio,350*Xratio,450*Yratio);
    for (int i=0; i<4; i++)
	{
        for(int j=0; j<4; j++)
		{
            if(ar[i][j]!=0)
            {
            	switch(ar[i][j])
            	{
            		case 2:
            			readimagefile("resources/2048/2.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 4:
            			readimagefile("resources/2048/4.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 8:
            			readimagefile("resources/2048/8.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 16:
            			readimagefile("resources/2048/16.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 32:
            			readimagefile("resources/2048/32.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 64:
            			readimagefile("resources/2048/64.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 128:
            			readimagefile("resources/2048/128.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 256:
            			readimagefile("resources/2048/256.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 512:
            			readimagefile("resources/2048/512.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
           			case 1024:
           				readimagefile("resources/2048/1024.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
           			case 2048:
           				readimagefile("resources/2048/2048.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
           			case 4096:
           				readimagefile("resources/2048/4096.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
           			case 8192:
           				readimagefile("resources/2048/8192.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
           			case 16384:
           				readimagefile("resources/2048/16384.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
           			case 32768:
           				readimagefile("resources/2048/32768.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 65536:
            			readimagefile("resources/2048/65536.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		case 131072:
            			readimagefile("resources/2048/1131072.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);
            			break;
            		default:
            			readimagefile("resources/2048/god_mode.jpg",grid_ref_x[i],grid_ref_y[j],grid_ref_x[i+1],grid_ref_y[j+1]);	
            			break;
				}
			}
        }
    }
}
int checklose_2048(int ar[4][4])
{
    int count=0;
    for (int i=0; i<4; i++)
	{
        for(int j=0; j<4; j++)
		{
            if(ar[i][j]==0)
			{
                count++;
            }
        }
    }
    if(count==0)
	{
        for (int i=0; i<4; i++)
		{
            for(int j=0; j<4; j++)
			{
                if((j!=3)&&ar[i][j]==ar[i][j+1])
				{
                    return 0;
                }
				else if((i!=3)&&ar[i][j]==ar[i+1][j])
				{
                    return 0;
                }
            }
        }
    }
	else
	{
        return 0;
    }
    return 1;
}
int win_menu_2048(int ar[4][4])
{
	int choice=1;
	display_2048(ar);
	while(1)
	{
		setactivepage(page);
		setvisualpage(1-page);
		if (GetAsyncKeyState(VK_UP)||GetAsyncKeyState(VK_DOWN))
		{
			choice=1-choice;
		}
		else if(GetAsyncKeyState(VK_RETURN))
		{
			return choice;
		}
		else if(GetAsyncKeyState(VK_RETURN))
			return choice;
		switch(choice)
		{
			case 0:
				readimagefile("resources/2048/win_quit.jpg",475*Xratio,250*Yratio,725*Xratio,550*Yratio);
				break;
			case 1:
			default:
				readimagefile("resources/2048/win_continue.jpg",475*Xratio,250*Yratio,725*Xratio,550*Yratio);
		}
		page=1-page;
		delay(150);
	}
}
int lose_menu_2048(int ar[4][4])
{
	int choice=1;
	setactivepage(page);
	setvisualpage(1-page);
	display_2048(ar);
	page=1-page;
	while(1)
	{
		setactivepage(page);
		setvisualpage(1-page);
		if (GetAsyncKeyState(VK_UP)||GetAsyncKeyState(VK_DOWN))
		{
			choice=1-choice;
		}
		else if(GetAsyncKeyState(VK_RETURN))
		{
			return choice;
		}
		switch(choice)
		{
			case 0:
				readimagefile("resources/2048/lose_quit.jpg",875*Xratio,250*Yratio,1225*Xratio,550*Yratio);
				break;
			case 1:
			default:
				readimagefile("resources/2048/lose_reset.jpg",875*Xratio,250*Yratio,1225*Xratio,550*Yratio);
		}
		page=1-page;
		delay(150);
	}
}
int options_2048(int ar[4][4])
{
	int choice=1;
	display_2048(ar);
	while(1)
	{
		setactivepage(page);
		setvisualpage(1-page);
		if (GetAsyncKeyState(VK_UP)||GetAsyncKeyState(VK_DOWN))
		{
			choice=1-choice;
		}
		else if(GetAsyncKeyState(VK_ESCAPE))
		{
			return 2;
		}	
		else if(GetAsyncKeyState(VK_RETURN))
		{
			return choice;
		}
		switch(choice)
		{
			case 0:
				readimagefile("resources/2048/options_quit.jpg",475*Xratio,250*Yratio,725*Xratio,550*Yratio);
				break;
			case 1:
			default:
				readimagefile("resources/2048/options_reset.jpg",475*Xratio,250*Yratio,725*Xratio,550*Yratio);
		}
		page=1-page;
		delay(100);
	}
}
void game_2048()
{
	srand(time(0));
    int x=rand()%4,y=rand()%4;
    int ar[4][4];
    for (int i=0; i<4; i++)
	{
        for(int j=0; j<4; j++)
		{
            ar[i][j]=0;
        }
    }
    ar[x][y]=2;
    display_2048(ar);
    page=1-page;
    while(1)
	{
		setactivepage(page);
		setvisualpage(1-page);
        if(win_continue==0&&win_check_2048(ar)==1)	//player wins
		{
			if(win_menu_2048(ar)==0)					//player presses quit
			{
				break;
			}
			else								//player presses continue
			{
				display_2048(ar);
				page=1-page;
				continue;
			}
        }
        if(caller_2048(ar)==0)						//player presses Escape
        {
        	int option=options_2048(ar);
        	if(option==0)						//player presses quit
        	{
        		break;
        	}
        	else if(option==1)					//player presses reset
        	{
        		score=0;
        		for (int i=0; i<4; i++)
				{
        			for(int j=0; j<4; j++)
					{
            			ar[i][j]=0;
       				}
    			}
    			generator_2048(ar);
       			change=0;
			}
		}
        if(change)								//generate new block of 2 only if there is a change the squares
		{
            generator_2048(ar);
            change=0;
        }
        if(checklose_2048(ar)==1)					//if the player loses
		{
        	display_2048(ar);
            delay(3000);
            page=1-page;
            if(lose_menu_2048(ar)==1)				//option for reset
            {
            	score=0;
        		for (int i=0; i<4; i++)
				{
        			for(int j=0; j<4; j++)
					{
            			ar[i][j]=0;
       				}
    			}
    			generator_2048(ar);
       			change=0;
			}									//option for quit
			else
			{
				break;
			}
        }
        display_2048(ar);
        delay(50);
        page=1-page;
    }
}
void start_2048(){
	cleardevice();
	setbkcolor(WHITE);
	cleardevice();
	setcolor(BLACK);
	setlinestyle(0,0,3);
	settextstyle(3,0,7);
	game_2048();
}
