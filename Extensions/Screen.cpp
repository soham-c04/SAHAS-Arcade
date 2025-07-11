#include "Screen.h"
using namespace std;

const double pi=2*acos(0.0);
double rad(int degree){
	double ans=pi*degree/180.0;
	return ans;
}
double deg(double radi){
	double ans=180.0*radi/pi;
	return ans;
}

int width = 1280;		// [GetSystemMetrics(SM_CXSCREEN)] Gets the X resolution of Screen
int height= 720;		// [GetSystemMetrics(SM_CYSCREEN)] Gets the Y resolution of Screen
const int X_full=width, Y_full=height; 	  // Stores the X and Y orignial resolution of screen used in Xratio and Yratio
int X1, Y1; 		    // coordinates of top left corner of playable field
int X2, Y2; 		    // coordinates of bottom right corner of playable field
int X=10, Y=100*Yratio; // Non-playable part of field

const int DELAY=10;    	// Time difference between 2 frames (in ms)
int page;               // Active and Visual page for double buffering
