#include "glut.h"
#include <math.h>
#include <time.h>


const int W = 800;
const int H = 600;
const double PI = 3.14;

const int FLAG_RED{ 130 };
const int FLAG_GREEN{ 124 };
const int FLAG_BLUE{ 200 };


// simulates Frame Buffer
unsigned char pixels[H][W][3] = { 0 }; // 3 is for RGB

double offset{ 0 };

void init()
{
	int i, j;
	//srand(time(0));
	glClearColor(0, 0, 0, 0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // defines the main axis and their range

}

void DrawLine(int fromX, int fromY, int  toX, int toY, int thickness, int red, int green, int blue)
{
	if (thickness <= 0) // recursion stop codition
		return;

	if (fromX == toX) // taking care of vertical lines
	{
		if (fromY > toY) //that way the for loop below wont work , so we switch the indexes
			DrawLine(toX, toY, fromX, fromY, thickness, red, green, blue);
		for (int curY{ fromY }; curY < toY; curY++)
		{
			pixels[curY][fromX][0] += red;
			pixels[curY][fromX][1] += green;
			pixels[curY][fromX][2] += blue;
		}
		DrawLine(fromX + 1, fromY, toX + 1, toY, thickness - 1, red, green, blue); // recursion
	}

	// horizonal / virgule lines case 
	if (fromX > toX) //that way the for loop below wont work , so we switch the indexes
		DrawLine(toX, toY, fromX, fromY, thickness, red, green, blue);
	int curY;
	double m = (fromY - static_cast<double>(toY)) / (fromX - static_cast<double>(toX));
	double n = fromY - m * fromX;
	for (int curX{ fromX }; curX < toX; curX++)
	{
		curY = m * curX + n;
		pixels[curY][curX][0] = red;
		pixels[curY][curX][1] = green;
		pixels[curY][curX][2] = blue;
	}
	if (m != 0)// virgule lines case
		DrawLine(fromX + 1, fromY, toX + 1, toY, thickness - 1, red, green, blue); //recursion
	else // horizonal lines case
		DrawLine(fromX, fromY + 1, toX, toY + 1, thickness - 1, red, green, blue); //recursion


}

void createFlag()
{
	int i, j;
	// white BG
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			pixels[i][j][0] = 255; // red
			pixels[i][j][1] = 255; // green
			pixels[i][j][2] = 255; // blue
		}
	// lower strip
	for (i = H * 0.15; i < H * 0.3; i++)
		for (j = 0; j < W; j++)
		{
			pixels[i][j][0] = FLAG_RED; // red
			pixels[i][j][1] = FLAG_GREEN; // green
			pixels[i][j][2] = FLAG_BLUE; // blue
		}
	// upper strip
	for (i = H * 0.85; i > H * 0.7; i--)
		for (j = 0; j < W; j++)
		{
			pixels[i][j][0] = FLAG_RED; // red
			pixels[i][j][1] = FLAG_GREEN; // green
			pixels[i][j][2] = FLAG_BLUE; // blue
		}
	DrawLine(W * 0.3, H * 0.575, W * 0.7 + 10, H * 0.575, 3, FLAG_RED, FLAG_GREEN, FLAG_BLUE); //down pointing triangle
	DrawLine(W * 0.7 + 5, H * 0.575, W * 0.5, H * 0.35, 5, FLAG_RED, FLAG_GREEN, FLAG_BLUE);
	DrawLine(W * 0.5, H * 0.35, W * 0.3, H * 0.575, 5, FLAG_RED, FLAG_GREEN, FLAG_BLUE);

	DrawLine(W * 0.3, H * 0.425, W * 0.7 + 10, H * 0.425, 3, FLAG_RED, FLAG_GREEN, FLAG_BLUE); //up pointing triangle
	DrawLine(W * 0.7 + 10, H * 0.425, W * 0.5, H * 0.65, 5, FLAG_RED, FLAG_GREEN, FLAG_BLUE);
	DrawLine(W * 0.5, H * 0.65, W * 0.3 - 5, H * 0.425, 5, FLAG_RED, FLAG_GREEN, FLAG_BLUE);


}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	createFlag();
	int i, j;
	double dist;
	double waveC;
	offset += 0.25;
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			dist = sqrt(pow(i + H / 2.0, 2) + pow(j + W / 2.0, 2));
			waveC = (1 + sin(dist / 100 - offset)) / 2 * (1 + sin(dist / 30 - offset)) / 2;
			pixels[i][j][0] += 160 + 55 * waveC; // red
			pixels[i][j][1] += 160 + 85 * waveC; // green
			pixels[i][j][2] += 160 + 95 * waveC; // blue

		}
	// draws the matrix pixels
	glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	glutSwapBuffers(); // show all
}

void idle()
{

	glutPostRedisplay(); // sends a message to the window to run display function
}


void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Waving Flag");

	glutDisplayFunc(display); // display is a name of refresh function
	glutIdleFunc(idle);// idle is a function that runs in background

	init();

	glutMainLoop();
}