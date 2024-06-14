#include "iGraphics.h"

// Game constants and variables
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define BALL_RADIUS 10 


// Background Color
int bg_color_red = 128;    // Default color is Gray
int bg_color_green = 128;
int bg_color_blue = 128;

// Paddle Color
int pad_color_red = 0;
int pad_color_green = 0;   // Default color is Navy
int pad_color_blue = 128;

// Ball Color
int ball_color_red = 0;
int ball_color_green = 0;
int ball_color_blue = 0;   // Default color is Black

struct position
{
	double x;
	double y;
}ball_position, paddle_position;


double marker=0.59;
double settings_marker = 0.59;
double in_settings_marker = 0.59;
double speed = 0.2;
double x_direction = 1;  // +ve direction for right
double y_direction = 1;  // +ve direction for up


bool menu = true;
bool in_game = false;
bool settings = false;
bool high_score = false;
bool background_settings = false;
bool ball_settings = false;
bool paddle_settings = false;
bool game_started = false;



//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Idraw Here::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://

void iDraw()
{
	
	iClear();
	// Game menu
	if (menu == true) {
		
		iSetColor(255, 255, 255);
		iFilledRectangle(0.04*WINDOW_WIDTH, marker*WINDOW_HEIGHT, 10, 30);
		iSetColor(0, 255, 0);
		iText(0.45*WINDOW_WIDTH, 0.9*WINDOW_HEIGHT, "DX Ball", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(0, 255, 0);
		iText(0.05*WINDOW_WIDTH, 0.6*WINDOW_HEIGHT, "New Game", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(0, 255, 0);
		iText(0.05*WINDOW_WIDTH, 0.5*WINDOW_HEIGHT, "High Scores", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(0, 255, 0);
		iText(0.05*WINDOW_WIDTH, 0.4*WINDOW_HEIGHT, "Settings", GLUT_BITMAP_TIMES_ROMAN_24);
	
	}

	if (settings == true) {

		iSetColor(255, 255, 255);
		iFilledRectangle(0.04*WINDOW_WIDTH, settings_marker*WINDOW_HEIGHT, 10, 30);
		iSetColor(0, 255, 0);
		iText(0.05*WINDOW_WIDTH, 0.6*WINDOW_HEIGHT, "Background", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(0, 255, 0);
		iText(0.05*WINDOW_WIDTH, 0.5*WINDOW_HEIGHT, "Paddle", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(0, 255, 0);
		iText(0.05*WINDOW_WIDTH, 0.4*WINDOW_HEIGHT, "Ball", GLUT_BITMAP_TIMES_ROMAN_24);
		
		// Shows current background 
		iSetColor(bg_color_red, bg_color_green, bg_color_blue);
		iFilledRectangle(0.5*WINDOW_WIDTH, 0, 0.5*WINDOW_WIDTH, WINDOW_HEIGHT);

		// Shows current paddle
		iSetColor(pad_color_red, pad_color_green, pad_color_blue);
		iFilledRectangle(0.75*WINDOW_WIDTH - 0.5*PADDLE_WIDTH, 10, PADDLE_WIDTH, PADDLE_HEIGHT);

		// Shows current ball
		iSetColor(ball_color_red, ball_color_green, ball_color_blue);
		iFilledCircle(0.75*WINDOW_WIDTH- 0.5*PADDLE_WIDTH + 0.5*PADDLE_WIDTH, 10 + PADDLE_HEIGHT + BALL_RADIUS, BALL_RADIUS, 100);

	}

	// This is in game code
	if (in_game == true) {
		
		// Shows background 
		iSetColor(bg_color_red, bg_color_green, bg_color_blue);
		iFilledRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		if (game_started == true) {
			// Shows paddle
			iSetColor(pad_color_red, pad_color_green, pad_color_blue);
			iFilledRectangle(paddle_position.x, paddle_position.y, PADDLE_WIDTH, PADDLE_HEIGHT);

			// change ball co-ordinate
			ball_position.x += speed*x_direction;
			if (ball_position.x > WINDOW_WIDTH) {
				x_direction = -x_direction;
			}
			if (ball_position.x < 0) {
				x_direction = -x_direction;
			}
			
			ball_position.y += speed*y_direction;
			if (ball_position.y > WINDOW_HEIGHT) {
				y_direction = -y_direction;
			}
			if ((ball_position.y - (10+PADDLE_HEIGHT+BALL_RADIUS))<0.1) {
				//debug
				//y_direction = -y_direction;
				if (ball_position.x >= paddle_position.x && ball_position.x <= (paddle_position.x+PADDLE_WIDTH)) {
					y_direction = -y_direction;
				}
			}
			

			// Shows ball
			iSetColor(ball_color_red, ball_color_green, ball_color_blue);
			iFilledCircle(ball_position.x, ball_position.y, BALL_RADIUS, 100);
		}

		if (game_started != true) {
			// Shows paddle
			iSetColor(pad_color_red, pad_color_green, pad_color_blue);
			iFilledRectangle(paddle_position.x, paddle_position.y, PADDLE_WIDTH, PADDLE_HEIGHT);

			// Shows ball
			iSetColor(ball_color_red, ball_color_green, ball_color_blue);
			iFilledCircle(ball_position.x, ball_position.y, BALL_RADIUS, 100);
			
			// Press any key to start
			iSetColor(255, 255, 255);
			iText(0.4*WINDOW_WIDTH, 0.8*WINDOW_HEIGHT, "Press any key to start", GLUT_BITMAP_TIMES_ROMAN_24);
		}


	}

}





/*function iMouseMove() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/


void iMouseMove(int mx, int my)
{

}
//*******************************************************************ipassiveMouse***********************************************************************//
void iPassiveMouseMove(int mx, int my)
{
	
}

void iMouse(int button, int state, int mx, int my)
{

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		
	}
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/


void iKeyboard(unsigned char key)
{
	if (in_game == true) {
		if (game_started != true) {
			game_started = true;
		}
		
	}
	if (menu == true) {
		if (key == '\r')  // 'r' ==> enter
		{
			if (marker == 0.59) {
				menu = false;
				
				ball_position.x = 0.5*WINDOW_WIDTH - 0.5*PADDLE_WIDTH + 0.5*PADDLE_WIDTH;
				ball_position.y = 10 + PADDLE_HEIGHT + BALL_RADIUS;

				paddle_position.x = 0.5*WINDOW_WIDTH - 0.5*PADDLE_WIDTH;
				paddle_position.y = 10;

				in_game = true;
			}

			else if (marker == 0.49) {
				menu = false;
				high_score = true;
			}
			else if (marker == 0.39) {
				menu = false;
				settings = true;
			}
		}
	}

	if (settings == true) {
		if (key == '\r')  // 'r' == enter
		{
			if (settings_marker == 0.59) {
				background_settings = true;
			}

			else if (settings_marker == 0.49) {
				paddle_settings = true;
			}
			else if (settings_marker == 0.39) {
				ball_settings = true;
			}
		}
	}
	
}

/*
function iSpecialKeyboard() is called whenver user hits special keys like-
function keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

	if (in_game == true) {
		if (game_started != true) {
			game_started = true;
		}
		if (game_started == true) {
			if (key == GLUT_KEY_LEFT) {
				if (paddle_position.x > 0) {
					paddle_position.x -= 25;
				}
			}
			if (key == GLUT_KEY_RIGHT) {
				if (paddle_position.x < WINDOW_WIDTH-PADDLE_WIDTH) {
					paddle_position.x += 25;
				}
			}
		}
	}

	if (menu == true) {
		if (key == GLUT_KEY_DOWN) {
			if (marker == 0.39) {
				marker = 0.59;
			}
			else {
				marker -= 0.1;
			}

		}

		if (key == GLUT_KEY_UP) {
			if (marker == 0.59) {
				marker = 0.39;
			}
			else {
				marker += 0.1;
			}

		}
	}

	if (settings == true) {
		if (key == GLUT_KEY_DOWN) {
			if (settings_marker == 0.39) {
				settings_marker = 0.59;
			}
			else {
				settings_marker -= 0.1;
			}

		}

		if (key == GLUT_KEY_UP) {
			if (settings_marker == 0.59) {
				settings_marker = 0.39;
			}
			else {
				settings_marker += 0.1;
			}

		}
	}
}


int main()
{
	///srand((unsigned)time(NULL));
	iInitialize(WINDOW_WIDTH,WINDOW_HEIGHT, "DX Ball");
	///updated see the documentations
	iStart();
	return 0;
}