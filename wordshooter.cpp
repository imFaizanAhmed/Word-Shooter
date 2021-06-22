//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain and Faizan Ahmed
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//desktop sae
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include "util.h"

using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 1000000000000000

string * dictionary;
int dictionarysize = 370099;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30,Size=15; // ball radius in pixels...
int A[10][15],B[Size];//arrays to store random alphabets
char sto_alp[10][15];      //it contain the display alphabets rathen then their number
int width = 930, height = 660;
int nrows = 9;
int byoffset = bradius;
int sec=60;//sec starts
int sec_con=0;//sec controler
int nmx=470,nmy=10;//new mouse coordinates...
double x=470.0,y=10.0;           //initial coordinates of alphabet...
int N=0,B_cont=0;                   //index of array that  control upcoming random alphabet
int cont=0;                           //use for swipping
int row=0,col=0;
float sq1,dy,sq2,sq3,sq;    	   //for ball shooting           
int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2;                            // initially number of full rows //
float score = 0,dx;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...


//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}


void check(int row,int col){
	for(int i=0;i<10;i++){                    //convert the alphabet in numbers
		for(int j=0;j<15;j++){
			switch(A[i][j]){
				case(0):
					sto_alp[i][j]='a';	
					break;
				case(1):
					sto_alp[i][j]='b';	
					break;
				case(2):
					sto_alp[i][j]='c';	
					break;
				case(3):
					sto_alp[i][j]='d';	
					break;
				case(4):
					sto_alp[i][j]='e';	
					break;
				case(5):
					sto_alp[i][j]='f';	
					break;
				case(6):
					sto_alp[i][j]='g';	
					break;
				case(7):
					sto_alp[i][j]='h';	
					break;
				case(8):
					sto_alp[i][j]='i';	
					break;
				case(9):
					sto_alp[i][j]='j';	
					break;
				case(10):
					sto_alp[i][j]='k';	
					break;
				case(11):
					sto_alp[i][j]='l';	
					break;
				case(12):
					sto_alp[i][j]='m';	
					break;
				case(13):
					sto_alp[i][j]='n';	
					break;
				case(14):
					sto_alp[i][j]='o';	
					break;
				case(15):
					sto_alp[i][j]='p';	
					break;
				case(16):
					sto_alp[i][j]='q';	
					break;
				case(17):
					sto_alp[i][j]='r';	
					break;
				case(18):
					sto_alp[i][j]='s';	
					break;
				case(19):
					sto_alp[i][j]='t';	
					break;
				case(20):
					sto_alp[i][j]='u';	
					break;
				case(21):
					sto_alp[i][j]='v';	
					break;
				case(22):
					sto_alp[i][j]='w';	
					break;
				case(23):
					sto_alp[i][j]='x';	
					break;
				case(24):
					sto_alp[i][j]='y';	
					break;
				case(25):
					sto_alp[i][j]='z';	
					break;
				case(26):
					sto_alp[i][j]=' ';	
					break;

			}
		}
	}	
	int alp_len=0;                       
	for(int i=0;i<370099;i++){               //find the length of strings
		alp_len=0;
		for(int j=0;true;j++,alp_len++){
			if(dictionary[i][j]=='\0')
				break;
		}
		int ind_chk=0;
		if(alp_len>2 && alp_len<10){                            //find combination in row forward direction
			for(int k=0;k<alp_len;k++){
				if((col+k)==16){                        //column out of range 
					ind_chk=0;
					break;
				}
				if(sto_alp[row][col+k]!=dictionary[i][k]){ 
					ind_chk=0;
					break;
				}
				ind_chk++;
			}
			if(ind_chk!=0){                                 //if alphabet match
				for(int l=0;l<ind_chk;l++){             //brust
					A[row][col+l]=26;
					sto_alp[row][col+l]=' ';
					score++;
				}
			}
		}
	
		ind_chk=0;
		if(alp_len>2 && alp_len<10){                            //find combination in row backward direction
			for(int k=0;k<alp_len;k++){
				if((col-k)==-1){                        //column out of range from left side
					ind_chk=0;
					break;
				}
				if(sto_alp[row][col-k]!=dictionary[i][k]){
					ind_chk=0;
					break;
				}
				ind_chk++;
			}
			if(ind_chk!=0){                                 //if alphabet match
				for(int l=0;l<ind_chk;l++){		//brust
					A[row][col-l]=26;
					sto_alp[row][col-l]=' ';
					score++;
				}
			}
		}
	ind_chk=0;
		if(alp_len>2 && alp_len<10){
			for(int k=0;k<alp_len;k++){	                //row goes out of range 
				if((row-k)==-1){
					ind_chk=0;
					break;
				}
				if(sto_alp[row-k][col]!=dictionary[i][k]){
					ind_chk=0;
					break;
				}
				ind_chk++;
			}
			if(ind_chk!=0){                                 //if alphabet match
				for(int l=0;l<ind_chk;l++){		//brust
					A[row-l][col]=26;
					sto_alp[row-1][col]=' ';
					score++;
				}
			}
		}
		ind_chk=0;
		if(alp_len>2 && alp_len<10){	//check anti-diagonally
			for(int k=0;k<alp_len;k++){                      //row out of range
				if((row-k)==-1){
					ind_chk=0;
					break;
				}
				if((col-k)==-1){                        //column out of range from left side

					ind_chk=0;

					break;

				}
				if(sto_alp[row-k][col-k]!=dictionary[i][k]){
					ind_chk=0;
					break;
				}
				ind_chk++;
			}
			if(ind_chk!=0){                                  //if alphabet match
				for(int l=0;l<ind_chk;l++){		//brust
					A[row-l][col-l]=26;		
					sto_alp[row-1][col-l]=' ';
					score++;
				}
			}
		}
		ind_chk=0;
		if(alp_len>2 && alp_len<10){		//check diagonally
			for(int k=0;k<alp_len;k++){                         //row out of range
				if((row-k)==-1){
					ind_chk=0;
					break;
				}
				if((col+k)==16){                        //column out of range 
					ind_chk=0;
					break;
				}
				if(sto_alp[row-k][col]!=dictionary[i][k]){
					ind_chk=0;
					break;
				}
				ind_chk++;
			}
			if(ind_chk!=0){                                  //if alphabet match
				for(int l=0;l<ind_chk;l++){		//brust
					A[row-l][col+l]=26;
					sto_alp[row-1][col]=' ';
					score++;
				}
			}
		}
	}
}	


void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet() {
	return GetRandInRange(0, 26);
}

void P2C(){					//convert the pixels in cells
	y+=60;
	x+=60;
	if(y<height-520)
		row=8;
	else if(y<height-460)
		row=7;
	else if(y<height-400)
		row=6;
	else if(y<height-340)
		row=5;
	else if(y<height-280)
		row=4;
	else if(y<height-220)
		row=3;
	else if(y<height-160)
		row=2;
	else if(y<height-100) 
		row=1;
	else if(y<height-40)
		row=0;
	if	(x<70)
		col=0;
	else if(x<130)
		col=1;
	else if(x<190)
		col=2;
	else if(x<250)
		col=3;
	else if(x<310)
		col=4;
	else if(x<370)
		col=5;
	else if(x<430)
		col=6;
	else if(x<490)
		col=7;
	else if(x<550)
		col=8;
	else if(x<610)
		col=9;
	else if(x<670)
		col=10;
	else if(x<730)
		col=11;
	else if(x<790)
		col=12;
	else if(x<850)
		col=13;
	else if(x<910)
		col=14;
	y-=60;
	x-=60;		
}


void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
	
	
	
		
}
void end() {
	
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	DrawAlphabet((alphabets)(6), 130, height-280 , awidth, aheight);
	DrawAlphabet((alphabets)(0), 190, height-280 , awidth, aheight);
	DrawAlphabet((alphabets)(12), 250, height-280, awidth, aheight);
	DrawAlphabet((alphabets)(4), 310, height-280 , awidth, aheight);
	DrawAlphabet((alphabets)(26), 370, height-280 , awidth, aheight);
	DrawAlphabet((alphabets)(14), 430, height-280 , awidth, aheight);
	DrawAlphabet((alphabets)(21), 490, height-280 , awidth, aheight);
	DrawAlphabet((alphabets)(4), 550, height-280 , awidth, aheight);
	DrawAlphabet((alphabets)(17),610,  height-280,  awidth, aheight);
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	DrawString(330, height - 330, width, height + 5, "totle Score  ==   " /*+ Num2Str(score)*/, colors[BLUE_VIOLET]);
	glutSwapBuffers();
	cin.get();
	
}

//function which display the end page
/*
* Main Canvas drawing function.
* */
void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	//write your drawing commands here or call your drawing functions...
	int py = 100;
	for(int j=0; j < nrows; j++){
		int px = 10;//control the location of alphabet in horizental direction
		for(int i=0;i<Size;i++){					//display the array
			DrawAlphabet((alphabets)(A[j][i]), px, height-py , awidth, aheight);
			px+=60;
		}
		py+=60;
	}

	if(y==nmy){						//control the next ball in mid of endline
		x=470;
		y=10;
		nmx=470;
		nmy=10;
		DrawAlphabet((alphabets)(B[N+1]),470 , bheight , awidth, aheight);        //display the ball at mid of last line
		DrawAlphabet((alphabets)(B[N+2]),0, 10 , awidth, aheight);                //display the next ball
	}
	else{						//control the smoothness of shoot
		DrawAlphabet((alphabets)(B[N+1]),0, 10 , awidth, aheight);
		y+=(3*dy);
		x+=(3*dx); 
		if(x<=0 || x>=px-60)
			dx*=(-1);
			P2C();                                                    //convert the pixels in cells 
		if(A[row][col]!=26){						  //store the shoot ball in array
			if(A[row+1][col]==26){
				A[row+1][col]=B[N];
				check(row+1,col);
			}
			else if(x>470){
				A[row][col-1]=B[N];
				check(row,col-1);
			}
			else if(x<470){
				A[row][col+1]=B[N];
				check(row,col+1);
			}				
			y=nmy;
		}
		DrawAlphabet((alphabets)(B[N]), x, y , awidth, aheight);
	}
	sec_con++;
	if (sec_con==70){
		sec--;
		sec_con=0;
	}
	DrawString(10, 80, width, height + 5, "next ball " /*+ Num2Str(0)*/, colors[BLACK]);
	DrawString(40, height - 20, width, height + 5, "Score "  /*Num2Str(score)*/, colors[BLUE_VIOLET]);
	DrawString(width / 2 - 30, height - 25, width, height,
		"Time Left:  secs" /*Num2Str(sec) +*/ , colors[RED]);
	
	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
}

/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
		}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == GLUT_UP)
		{		
			if(nmy==10 && nmx==470){
				if(x<470)
					nmx=x-60;
				else
					nmx=x;
			nmy=height-y;
			sq1=pow((y-10),2);
			sq2=pow((x-460),2);
			sq3=sq1+sq2;				//find the distance between points
			glBindTexture(GL_TEXTURE_2D, mtid[1]);
			sq=sqrt(sq3);
			dy=(nmy-10)/sq;				//change in the y coordinate
			dx=(nmx-460)/sq;			//change in the x coordinate
			B_cont++;
			N=B_cont%Size;	}	
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{

	}
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
	
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) {

	glutPostRedisplay();
	glutTimerFunc(1000.0/FPS, Timer, 0);
	if(sec<0 )
		end();
}

/*
* our gateway main function
* */
int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize]; 
	ReadWords("dictionary.txt", dictionary); // dictionary is an array of strings
	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets
	for(int i=0;i<2;i++){					//store the random to display
		for(int j=0;j<Size;j++){
			A[i][j]=GetAlphabet();
		}
	}

	for(int i=0;i<Size;i++)					//store upcoming alphabet
		B[i]=GetAlphabet();
	for(int i=2;i<10;i++){					
		for(int j=0;j<15;j++){
			A[i][j]=26;
		}
	}
	SetCanvasSize(width, height); // set the number of pixels...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Faizan's ITCs Word Shooter"); // set the title of our game window
	DisplayFunction();//Display the alphabets
	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	glutDisplayFunc(DisplayFunction);
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */
