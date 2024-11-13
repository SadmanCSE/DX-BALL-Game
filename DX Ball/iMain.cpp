#include <iostream>
#include <cstdio> // Include cstdio for sprintf
#include <cmath> // Include cmath for cos and sin functions
#include <cstdlib> // Include cstdlib for rand
using namespace std;
#include "iGraphics.h"
#include <string>

#define BRICK_ROWS 10
#define BRICK_COLS 24
#define TOTAL_BRICKS (BRICK_ROWS * BRICK_COLS)
#define BRICK_WIDTH 40
#define BRICK_HEIGHT 20
#define BRICK_GAP 2
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 650
#define MAX_ENEMIES 6
#define PADDLE_INITIAL_LIFE 10
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 10
#define BULLET_WIDTH 5
#define MAX_BULLETS 10
#define MAX_PERKS 20

enum PerkType {
	EXPAND_PADDLE, SHRINK_PADDLE, EXTRA_LIFE, SLOW_BALL, FAST_BALL,
	THRU_BRICK, FIREBALL, SHOOTING_PADDLE, GRAB_PADDLE, MULTIPLY_BALL,
	LEVEL_WARP, ZAP_BRICKS, SET_OFF_EXPLODING, FALLING_BRICKS, KILL_PADDLE,
	SPLIT_BALL, SUPER_SHRINK, NONE
};

#define yAxisMargin 50
#define qetaDefine 0
#define PI 3.14159265
int mposx, mposy;
int frames = 0;

// Brick structure:
struct Brick {
	int x, y;
	int dx = BRICK_WIDTH, dy = BRICK_HEIGHT;
	int red, green, blue;
	bool show = true;
};
Brick bar[TOTAL_BRICKS];

struct Enemy {
	int x, y;
	int life = 1; // Enemy initial life
	bool active = false;
};

struct Obstacle {
	int x, y;
	int dy = -5;
	bool active = false;
};

struct Perk {
	int x, y;
	PerkType type;
	bool active = false;
	int dy = -3; // Speed of the falling perk
};
Enemy enemies[MAX_ENEMIES];
Obstacle obstacles[MAX_ENEMIES];
Perk perks[MAX_PERKS];

struct Bullet {
	int x, y;
	bool active;
};
Bullet bullets[MAX_BULLETS];
bool shootingPaddleActive = false;
GLuint paddleImage, shootingPaddleImage, ballImage, fireballImage;
bool imagesLoaded = false;
bool fireballActive = false;
int fireballDuration = 0;
const int FIREBALL_DURATION_LIMIT = 300;

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


double marker = 0.59;
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
bool game_started = false;   // different
bool game_over = false;
bool game_finished = false;
int current_level = 1;
//int timer = 0;
int paddle_life = PADDLE_INITIAL_LIFE;

// Ball setting: 
int xBall = (int)WINDOW_WIDTH / 2;
int yBall = yAxisMargin;
int BALL_RADIUS = 10;
// pras style:
int v = 10;
double qeta = 30.0;
int dx = (int)v*cos(qeta*PI / 180);
int dy = (int)v*sin(qeta*PI / 180);

// paddle settings: 
int dxBoard = 100;
int dyBoard = 10;
int xBoard = xBall - (dxBoard / 2);
int yBoard = yBall - 20;
int enemy_image; // Image ID for the enemy
int perk_images[17];

//Scores
int scores = 0;
int relative_score = 0;
int first = 0;
int second = 0;
int third = 0;

//Life
int br[TOTAL_BRICKS]; // Store the index of bricks hit
int brick_number = 0;
int br_enemy[MAX_ENEMIES];
int enemy_number = 0;

int Life = 3;

//Life photo
int x_size_Life = 20;
int y_size_Life = 20;
int x_position_Life = 0;
int y_position_Life = 600;

//Enemy count
int Enemy_Count = 0;


//game over menu

int white_box_x = 490;
int white_box_y = 245;

// Function prototypes
void ballChange();
void restartBall();
void score();
void score_showing();
void life_photo_showing();
void moveObstacles();
void obstacleFall();

void loadImages() {
	enemy_image = iLoadImage("enemy.png");
	perk_images[EXPAND_PADDLE] = iLoadImage("expand_paddle.png");
	perk_images[SHRINK_PADDLE] = iLoadImage("shrink_paddle.png");
	perk_images[EXTRA_LIFE] = iLoadImage("extra_life.png");
	perk_images[SLOW_BALL] = iLoadImage("slow_ball.png");
	perk_images[FAST_BALL] = iLoadImage("fast_ball.png");
	perk_images[THRU_BRICK] = iLoadImage("thru_brick.png");
	perk_images[FIREBALL] = iLoadImage("fireball.png");
	perk_images[SHOOTING_PADDLE] = iLoadImage("paddle_shooter.png");
	perk_images[GRAB_PADDLE] = iLoadImage("grab_paddle.png");
	perk_images[MULTIPLY_BALL] = iLoadImage("multiply_ball.png");
	perk_images[LEVEL_WARP] = iLoadImage("level_warp.png");
	perk_images[ZAP_BRICKS] = iLoadImage("zap_bricks.png");
	perk_images[SET_OFF_EXPLODING] = iLoadImage("set_off_exploding.png");
	perk_images[FALLING_BRICKS] = iLoadImage("falling_bricks.png");
	perk_images[KILL_PADDLE] = iLoadImage("kill_paddle.png");
	perk_images[SPLIT_BALL] = iLoadImage("split_ball.png");
	perk_images[SUPER_SHRINK] = iLoadImage("super_shrink.png");

	paddleImage = iLoadImage("paddle.png");
	shootingPaddleImage = iLoadImage("shooting_paddle.png");
	ballImage = iLoadImage("ball.png");
	fireballImage = iLoadImage("fireball_ball.png");
	imagesLoaded = true;
}
// Load paddle images
//void loadImage() {
	//paddleImage = iLoadImage("paddle.png");
	//shootingPaddleImage = iLoadImage("shooting_paddle.png");
	//imagesLoaded = true;
//}

void drawBricks() {
	for (int i = 0; i < TOTAL_BRICKS; i++) {
		if (bar[i].show) {
			iSetColor(bar[i].red, bar[i].green, bar[i].blue);
			iFilledRectangle(bar[i].x, bar[i].y, bar[i].dx, bar[i].dy);
		}
	}
}
void initializeLevel1() {
	
	int startX = 350;
	int startY = 600;
	int count = 0;
	for (int i = 0; i < BRICK_ROWS; i++) {
		for (int j = 0; j < BRICK_COLS; j++) {
			bar[count].x = startX + j * (BRICK_WIDTH + BRICK_GAP);
			bar[count].y = startY - i * (BRICK_HEIGHT + BRICK_GAP);
			if (i == 1 && j >= 4 && j <= 7) {
				bar[count].red = 24;
				bar[count].green = 222;
				bar[count].blue = 174;
			}
			else if (i == 2 && j >= 3 && j <= 8) {
				bar[count].red = 33;
				bar[count].green = 56;
				bar[count].blue = 50;
			}
			else if (i == 3 && j >= 4 && j <= 7) {
				bar[count].red = 50;
				bar[count].green = 91;
				bar[count].blue = 81;
			}
			else if (i == 4 && j >= 3 && j <= 8) {
				bar[count].red = 133;
				bar[count].green = 161;
				bar[count].blue = 154;
			}
			else if (i == 5 && j == 6) {
				bar[count].red = 90;
				bar[count].green = 153;
				bar[count].blue = 138;
			}
			else if (i == 6 && (j == 2 || j == 3 || j == 8 || j == 9)) {
				bar[count].red = 8;
				bar[count].green = 253;
				bar[count].blue = 194;
			}
			else if (i == 7 && (j >= 1 && j <= 4 || j >= 7 && j <= 10)) {
				bar[count].red = 142;
				bar[count].green = 249;
				bar[count].blue = 224;
			}
			else if (i == 8 && (j == 2 || j == 3 || j == 8 || j == 9)) {
				bar[count].red = 3;
				bar[count].green = 120;
				bar[count].blue = 92;
			}
			else {
				bar[count].show = false;
			}
			count++;
		}
	}
	// Initialize enemies for level 1
	int enemyCount = 0;
	for (int i = 0; i < TOTAL_BRICKS && enemyCount < MAX_ENEMIES; i++) {
		if (bar[i].show && rand() % 4 == 0) { // Randomly place enemies on some bricks
			enemies[enemyCount].active = true;
			enemies[enemyCount].x = bar[i].x + BRICK_WIDTH / 2;
			enemies[enemyCount].y = bar[i].y + BRICK_HEIGHT / 2;
			enemyCount++;
		}
	}
	Enemy_Count = enemyCount;
	
	
}

void initializeLevel2() {
	int startX = 100;
	int startY = 600;
	int count = 0;
	for (int i = 0; i < BRICK_ROWS; i++) {
		for (int j = 0; j < BRICK_COLS; j++) {
			bar[count].x = startX + j * (BRICK_WIDTH + BRICK_GAP);
			bar[count].y = startY - i * (BRICK_HEIGHT + BRICK_GAP);
			if (i % 2 == 0) {
				bar[count].red = 74;
				bar[count].green = 74;
				bar[count].blue = 232;
				bar[count].show = true;
			}
			else {
				bar[count].show = false;
			}
			count++;
		}
	}
	// Initialize enemies for level 2
	int enemyCount = 0;
	for (int i = 0; i < TOTAL_BRICKS && enemyCount < MAX_ENEMIES; i++) {
		if (bar[i].show && rand() % 4 == 0) { // Randomly place enemies on some bricks
			enemies[enemyCount].active = true;
			enemies[enemyCount].x = bar[i].x + BRICK_WIDTH / 2;
			enemies[enemyCount].y = bar[i].y + BRICK_HEIGHT / 2;
			enemyCount++;
		}
	}
	
	Enemy_Count = enemyCount;
	
	
}

void initializeLevel3() {
	int startX = 100;
	int startY = 600;
	int count = 0;
	for (int i = 0; i < BRICK_ROWS; i++) {
		for (int j = 0; j < BRICK_COLS; j++) {
			bar[count].x = startX + j * (BRICK_WIDTH + BRICK_GAP);
			bar[count].y = startY - i * (BRICK_HEIGHT + BRICK_GAP);
			if ((i + j) % 2 == 0) {
				bar[count].red = 247;
				bar[count].green = 144;
				bar[count].blue = 32;
				bar[count].show = true;
			}
			else {
				bar[count].show = false;
			}
			count++;
		}
	}
	// Initialize enemies for level 3
	int enemyCount = 0;
	for (int i = 0; i < TOTAL_BRICKS && enemyCount < MAX_ENEMIES; i++) {
		if (bar[i].show && rand() % 4 == 0) { // Randomly place enemies on some bricks
			enemies[enemyCount].active = true;
			enemies[enemyCount].x = bar[i].x + BRICK_WIDTH / 2;
			enemies[enemyCount].y = bar[i].y + BRICK_HEIGHT / 2;
			enemyCount++;
		}
	}
	
	Enemy_Count = enemyCount;
	
	
	
}

void initializeBricks() {
	switch (current_level) {
	case 1:
		initializeLevel1();
		break;
	case 2:
		initializeLevel2();
		break;
	case 3:
		initializeLevel3();
		break;
	default:
		initializeLevel1();
		break;
	}
	// Deactivate all obstacles
	for (int i = 0; i < MAX_ENEMIES; i++) {
		obstacles[i].active = false;
	}
}



void drawEnemies() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].active) {
			iShowImage(enemies[i].x - 10, enemies[i].y - 10, 20, 20, enemy_image); // Draw enemy image
		}
	}
}

void drawBullets() {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].active) {
			iSetColor(255, 0, 0);
			iFilledRectangle(bullets[i].x, bullets[i].y, 5, 10); // Bullet size
		}
	}
}

void fireBullet() {
	for (int i = 0; i < MAX_BULLETS / 2; i++) {
		if (!bullets[i].active) {
			bullets[i].active = true;
			bullets[i].x = xBoard; // Left edge
			bullets[i].y = yBoard + dyBoard;
			break;
		}
	}
	for (int i = 0; i < MAX_BULLETS / 2; i++) {
		if (!bullets[i].active) {
			bullets[i].active = true;
			bullets[i].x = xBoard + dxBoard; // Right edge
			bullets[i].y = yBoard + dyBoard;
			break;
		}
	}
}
void autoShoot() {
	if (shootingPaddleActive) {
		fireBullet();
	}
}
void updateBullets() {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].active) {
			bullets[i].y += 10; // Bullet speed
			if (bullets[i].y > WINDOW_HEIGHT) {
				bullets[i].active = false;
			}
			// Check collision with bricks
			for (int j = 0; j < TOTAL_BRICKS; j++) {
				if (bar[j].show && bullets[i].active &&
					bullets[i].x >= bar[j].x && bullets[i].x <= bar[j].x + BRICK_WIDTH &&
					bullets[i].y >= bar[j].y && bullets[i].y <= bar[j].y + BRICK_HEIGHT) {
					scores++;
					bar[j].show = false;
					br[brick_number++] = j;
					bullets[i].active = false;
					break;
				}
			}

			// Check for direct hit on enemies
			for (int k = 0; k < MAX_ENEMIES; k++) {
				if (bullets[i].active && bullets[i].x >= enemies[k].x - 10 && bullets[i].x <= enemies[k].x + 20 && bullets[i].y >= enemies[k].y- 10 && bullets[i].y <= enemies[k].y + 20) {
					enemies[k].life--;
					if (enemies[k].life <= 0) {
						enemies[k].active = false;
						br_enemy[enemy_number++] = k;
						
					}
					if (Enemy_Count >= 0){
						Enemy_Count--;
						
						
					}


				}
			}

		}
	}
}

void drawPaddle() {
	if (!imagesLoaded) {
		loadImages();
	}
	if (shootingPaddleActive) {
		// Draw the paddle with the shooting image
		iShowImage(xBoard, yBoard, dxBoard, dyBoard, shootingPaddleImage);
	}
	else{
		iShowImage(xBoard, yBoard, dxBoard, dyBoard, paddleImage);
	}
}

void drawBall() {
	if (!imagesLoaded) {
		loadImages();
	}
	if (fireballActive) {
		iShowImage(xBall - BALL_RADIUS, yBall - BALL_RADIUS, 2 * BALL_RADIUS, 2 * BALL_RADIUS, fireballImage);
	}
	else iShowImage(xBall - BALL_RADIUS, yBall - BALL_RADIUS, 2 * BALL_RADIUS, 2 * BALL_RADIUS, ballImage);
}

void drawObstacles() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (obstacles[i].active) {
			iSetColor(224, 41, 65);
			iFilledRectangle(obstacles[i].x, obstacles[i].y, 10, 10);
		}
	}
}

void drawLevel() {
	if (current_level <= 3){
		char levelText[20];
		sprintf_s(levelText, sizeof(levelText), "LEVEL: %d", current_level);
		iSetColor(255, 255, 255);
		iText(600, 630, levelText, GLUT_BITMAP_HELVETICA_18);

		char lifeText[20];
		sprintf_s(lifeText, sizeof(lifeText), "LIFE: %d", paddle_life);
		iSetColor(255, 255, 255);
		iText(10, 10, lifeText, GLUT_BITMAP_HELVETICA_18);
	}
}

void drawPerks() {
	for (int i = 0; i < MAX_PERKS; i++) {
		if (perks[i].active) {
			iShowImage(perks[i].x, perks[i].y, 30, 30, perk_images[perks[i].type]);
		}
	}
}

void updateFireball() {
	if (fireballActive) {
		fireballDuration++;
		if (fireballDuration >= FIREBALL_DURATION_LIMIT) {
			fireballActive = false;
		}
	}
}

void applyPerk(PerkType type) {
	switch (type) {
	case EXPAND_PADDLE:
		dxBoard += 60;
		break;
	case SHRINK_PADDLE:
		if (dxBoard > 40) dxBoard -= 40;
		break;
	case EXTRA_LIFE:
		if (Life<3){
			Life++;
		}
		break;
	case SLOW_BALL:
		if (dx > 3) dx -= 3;
		if (dy > 3) dy -= 3;
		break;
	case FAST_BALL:
		dx += 7;
		dy += 7;
		break;
	case THRU_BRICK:

		break;
	case FIREBALL:
		fireballActive = true;
		fireballDuration = 0;
		break;
	case SHOOTING_PADDLE:
		shootingPaddleActive = true;
		break;
	case GRAB_PADDLE:

		break;
	case MULTIPLY_BALL:

		break;
	case LEVEL_WARP:
		scores += 10;
		break;
	case ZAP_BRICKS:

		break;
	case SET_OFF_EXPLODING:

		break;
	case FALLING_BRICKS:
		paddle_life--;
		break;
	case KILL_PADDLE:
		paddle_life = 0;
		game_started = true;
		break;
	case SPLIT_BALL:

		break;
	case SUPER_SHRINK:
		if (dxBoard > 80) dxBoard -= 80;
		break;
	case NONE:
	default:
		break;

	
	}
}

void generatePerk(int brickIndex) {
	for (int i = 0; i < MAX_PERKS; i++) {
		if (!perks[i].active) {
			perks[i].x = bar[brickIndex].x + BRICK_WIDTH / 2;
			perks[i].y = bar[brickIndex].y;
			perks[i].type = static_cast<PerkType>(rand() % 17);	
		    perks[i].active = true;
			break;
			
		}
	}
}

/*

*/
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Idraw Here::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://

void iDraw()
{
	iClear();

	// Game menu
	if (menu == true) {
		iShowBMP(400,100,"menu.bmp");
		iSetColor(255, 255, 255);
		iFilledRectangle(0.04*WINDOW_WIDTH, marker*WINDOW_HEIGHT, 10, 30);
		iSetColor(255, 255, 255);
		iText(0.05*WINDOW_WIDTH, 0.6*WINDOW_HEIGHT, "New Game", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(255, 255, 255);
		iText(0.05*WINDOW_WIDTH, 0.5*WINDOW_HEIGHT, "High Scores", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(255, 255, 255);
		iText(0.05*WINDOW_WIDTH, 0.4*WINDOW_HEIGHT, "Settings", GLUT_BITMAP_TIMES_ROMAN_24);

	}
	if (settings == true) {

		iSetColor(255, 255, 255);
		iFilledRectangle(0.04*WINDOW_WIDTH, settings_marker*WINDOW_HEIGHT, 10, 30);
		iSetColor(255, 255, 255);
		iText(0.05*WINDOW_WIDTH, 0.6*WINDOW_HEIGHT, "Background", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(255, 255, 255);
		iText(0.05*WINDOW_WIDTH, 0.5*WINDOW_HEIGHT, "Paddle", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(255, 255, 255);
		iText(0.05*WINDOW_WIDTH, 0.4*WINDOW_HEIGHT, "Ball", GLUT_BITMAP_TIMES_ROMAN_24);

		// Shows current background
		iSetColor(bg_color_red, bg_color_green, bg_color_blue);
		iFilledRectangle(0.5*WINDOW_WIDTH, 0, 0.5*WINDOW_WIDTH, WINDOW_HEIGHT);

		// Shows current paddle
		iSetColor(pad_color_red, pad_color_green, pad_color_blue);
		iFilledRectangle(0.75*WINDOW_WIDTH - 0.5*PADDLE_WIDTH, 10, PADDLE_WIDTH, PADDLE_HEIGHT);

		// Shows current ball
		iSetColor(ball_color_red, ball_color_green, ball_color_blue);
		iFilledCircle(0.75*WINDOW_WIDTH - 0.5*PADDLE_WIDTH + 0.5*PADDLE_WIDTH, 10 + PADDLE_HEIGHT + BALL_RADIUS, BALL_RADIUS, 1000);

		if (background_settings) {

		}


	}

	if (in_game == true) {
		
		// Shows background
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		if (game_started == true) {
			// Ball Color:
			drawBall();
			//iSetColor(227, 40, 209);
			//iFilledCircle(xBall, yBall, BALL_RADIUS, 1000);

			// Paddle Color:
			//iSetColor(0, 255, 0);
			//iFilledRectangle(xBoard, yBoard, dxBoard, dyBoard);
			drawPaddle();


			// Update and draw bullets
			drawBullets();
			updateBullets();

			// Show Bricks:
			drawBricks();
			// Draw Level:
			drawLevel();
			// Enemy part: 
			drawEnemies();

			drawObstacles();
			// perk part:
			drawPerks();

			//Scores Updating
			score_showing();

			//Life Manupulation
			life_photo_showing();

			

		}
		if (game_started != true) {
			// Ball Color:
			drawBall();
			//iSetColor(227, 40, 209);
			//iFilledCircle(xBall, yBall, BALL_RADIUS, 1000);

			// Paddle Color:
			//iSetColor(0, 255, 0);
			//iFilledRectangle(xBoard, yBoard, dxBoard, dyBoard);
			drawPaddle();


			drawBricks();
			// Draw Level:
			drawLevel();
			// Press space to start

			drawEnemies();


			for (int i = 0; i < MAX_PERKS; i++) {
				if (perks[i].active) {
					perks[i].active = false;
				}
			}

			for (int i = 0; i < MAX_ENEMIES; i++) {
				obstacles[i].active = false;
			}

			for (int i = 0; i < MAX_BULLETS / 2; i++) {
				if (bullets[i].active) {
					bullets[i].active = false;
				}
			}

			shootingPaddleActive = false;
			fireballActive = false;
			dxBoard = 100;
			xBoard = xBall - (dxBoard / 2);

			if (Life == 3){
				iSetColor(255, 255, 255);
				iText(WINDOW_WIDTH*0.425, 0.1*WINDOW_HEIGHT, "Press space to start", GLUT_BITMAP_TIMES_ROMAN_24);
			}

			if (Life >= 1 && Life < 3){
				score_showing();
				life_photo_showing();
			}

		}
		
		
		

		

	}
	if (high_score == true){

		int first, second, third;
		FILE *file;
		fopen_s(&file, "BestScore.txt", "r");
		fscanf_s(file, "%i %i %i", &first, &second, &third);
		fclose(file);
		iSetColor(0, 255, 0);
		iText(0.45*WINDOW_WIDTH, 0.9*WINDOW_HEIGHT, "DX Ball", GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(0, 255, 0);
		iText(0.05*WINDOW_WIDTH, 0.6*WINDOW_HEIGHT, "1st.", GLUT_BITMAP_TIMES_ROMAN_24);
		char first_to_string[20];
		sprintf_s(first_to_string, sizeof(first_to_string), "%d", first);
		iText(0.05*WINDOW_WIDTH + 50, 0.6*WINDOW_HEIGHT, first_to_string, GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(0, 255, 0);
		iText(0.05*WINDOW_WIDTH, 0.5*WINDOW_HEIGHT, "2nd.", GLUT_BITMAP_TIMES_ROMAN_24);
		char second_to_string[20];
		sprintf_s(second_to_string, sizeof(second_to_string), "%d", second);
		iText(0.05*WINDOW_WIDTH + 50, 0.5*WINDOW_HEIGHT, second_to_string, GLUT_BITMAP_TIMES_ROMAN_24);


		iSetColor(0, 255, 0);
		iText(0.05*WINDOW_WIDTH, 0.4*WINDOW_HEIGHT, "Third.", GLUT_BITMAP_TIMES_ROMAN_24);
		char third_to_string[20];
		sprintf_s(third_to_string, sizeof(third_to_string), "%d", third);
		iText(0.05*WINDOW_WIDTH + 80, 0.4*WINDOW_HEIGHT, third_to_string, GLUT_BITMAP_TIMES_ROMAN_24);




	}
	if (game_over == true){
		iSetColor(0, 255, 0);
		iText(500, 325, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
		char score_to_string[5];
		sprintf_s(score_to_string, sizeof(score_to_string), "%d", relative_score);
		iText(500, 300, "Score:", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(600, 300,score_to_string, GLUT_BITMAP_TIMES_ROMAN_24);
			
		iSetColor(172, 255, 255);
		iText(500, 250, "PLAY AGAIN", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(500, 200, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(255, 255, 255);
		iFilledRectangle(white_box_x, white_box_y, 10, 30);
		
		
	}

	if (game_finished == true){

		iSetColor(175, 255, 0);
		iText(500, 325, "Congratulation", GLUT_BITMAP_TIMES_ROMAN_24);

		char score_to_string[5];
		sprintf_s(score_to_string, sizeof(score_to_string), "%d", relative_score);
		iText(500, 300, "Score:", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(600, 300, score_to_string, GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(255, 255, 255);
		iFilledRectangle(white_box_x, white_box_y, 10, 30);

		iSetColor(172, 255, 255);
		iText(500, 250, "PLAY AGAIN", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(500, 200, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
		
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
	if (game_started) xBoard = mx;
}

void iMouse(int button, int state, int mx, int my)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{


	}


	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/

bool yes = false;
void iKeyboard(unsigned char key)
{
	if (in_game == true){
		if (key ==' ')
		{
			if (!game_started){
				game_started = true;
			}
		}
	}
	if (key == 'x'){
		exit(0);
	}
	
	if (menu == true) {
		if (key == '\r')  // 'r' ==> enter
		{
			if (marker == 0.59) {
				menu = false;

				xBall = 0.5*WINDOW_WIDTH - 0.5*PADDLE_WIDTH + 0.5*PADDLE_WIDTH;
				yBall = 10 + PADDLE_HEIGHT + BALL_RADIUS;

				xBoard = 0.5*WINDOW_WIDTH - 0.5*PADDLE_WIDTH;
				yBoard = 10;

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
	if (game_over == true){
		if (white_box_y == 245 && key == '\r'){
			game_over = false;
			in_game = true;
		}
		else if (white_box_y == 195 && key == '\r'){
			game_over = false;
			menu = true;
		}
	}


	if (game_finished == true){

		if (white_box_y == 245 && key == '\r'){
			game_finished = false;
			in_game = true;
			

		}
		else if (white_box_y == 195 && key == '\r'){
			game_finished = false;
			menu = true;
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
	if (key == GLUT_KEY_HOME)
	{

	}

	if (in_game == true) {
		if (key == GLUT_KEY_LEFT)
		{
			if (xBoard > 0){
				if (game_started){
					xBoard -= 25;

				}

				if (!game_started && Life != 3)
				{
					xBall -= 25;
					xBoard -= 25;
				}
			}
		}
		if (key == GLUT_KEY_RIGHT)
		{
			if (xBoard < WINDOW_WIDTH - dxBoard){
				if (game_started){
					xBoard += 25;

				}
				if (!game_started && Life != 3)
				{
					xBall += 25;
					xBoard += 25;
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
		else if (key == GLUT_KEY_LEFT){
			settings = false;
			menu = true;

		}
	}
	if (high_score == true){
		if (key == GLUT_KEY_LEFT){
			high_score = false;
			menu = true;
		}
	}
	if (game_over == true){
		if (key == GLUT_KEY_DOWN && white_box_y==245){
			white_box_y -= 50;
		}
		else if (key = GLUT_KEY_UP && white_box_y == 195){
			white_box_y += 50;
		}
	}

	if (game_finished == true){
		if (key == GLUT_KEY_DOWN && white_box_y == 245){
			white_box_y -= 50;
		}
		else if (key = GLUT_KEY_UP && white_box_y == 195){
			white_box_y += 50;
		}
	}

}

void score_showing(){
	iSetColor(145, 238, 72);
	iText(0.9*WINDOW_WIDTH, 0.9*WINDOW_HEIGHT, "Score:", GLUT_BITMAP_TIMES_ROMAN_24);

	char score_to_string[20];
	sprintf_s(score_to_string, sizeof(score_to_string), "%d", scores);
	iText(0.95*WINDOW_WIDTH, 0.9*WINDOW_HEIGHT, score_to_string, GLUT_BITMAP_TIMES_ROMAN_24);
}

void life_photo_showing(){
	for (int i = 0; i < Life; i++){
		iShowBMP(x_size_Life + x_position_Life, y_position_Life, "Heart.bmp");
		x_position_Life += 20;
	}
	x_position_Life = 0;
}

void score(){

	FILE *file;
	fopen_s(&file, "BestScore.txt", "r");
	fscanf_s(file, "%i %i %i", &first, &second, &third);
	fclose(file);
	fopen_s(&file, "BestScore.txt", "w");
	if (scores > first){
		if (first > second){
			fprintf(file, "%i %i %i", scores, first, third);
		}
		else{
			if (first > third){
				fprintf(file, "%i %i %i", scores, second, first);
			}
			else{
				fprintf(file, "%i %i %i", scores, second, third);
			}
		}
	}
	else if (scores > second){
		if (second > third){
			fprintf(file, "%i %i %i", first, scores, second);
		}
		else{
			fprintf(file, "%i %i %i", first, scores, third);
		}

	}
	else if (scores > third){
		fprintf(file, "%i %i %i", first, second, scores);
	}

	fclose(file);

}


void ballChange(){
	if (game_started){
		xBall += dx;
		yBall += dy;

		if (xBall >= WINDOW_WIDTH || xBall <= 0) {
			dx = -dx;
		}
		if (yBall >= WINDOW_HEIGHT || yBall <= 0) {
			dy = -dy;
		}

		// Collision
		bool all_bricks_destroyed = true;
		for (int i = 0; i < TOTAL_BRICKS; i++) {
			if (bar[i].show) {
				all_bricks_destroyed = false;
				if (xBall >= bar[i].x && xBall <= bar[i].x + bar[i].dx &&
					yBall >= bar[i].y && yBall <= bar[i].y + bar[i].dy) {

					if (fireballActive) {
						bar[i].show = false; // Destroy brick without bouncing
						
					}
					else {
						dy = -dy;
						bar[i].show = false;
						
					}
					scores++;
					br[brick_number] = i;
					brick_number++;
					
					

					// Generate perk on brick destruction
					if (rand() % 3 == 0) { // 1 in 3 chance to generate a perk
						generatePerk(i);
					}

					break;
				}

				else if (yBall >= bar[i].y && yBall <= bar[i].y + bar[i].dy &&
					xBall >= bar[i].x && xBall <= bar[i].x + bar[i].dx) {

					if (fireballActive) {
						bar[i].show = false; // Destroy brick without bouncing
						
					}
					else {
						dx = -dx;
						bar[i].show = false;
						
					}

					scores++;
					br[brick_number] = i;
					brick_number++;
					
					

					// Generate perk on brick destruction
					if (rand() % 3 == 0) { // 1 in 3 chance to generate a perk
						generatePerk(i);
					}

					break;
				}
			}
		}

		// Move perks down
		for (int i = 0; i < MAX_PERKS; i++) {
			if (perks[i].active) {
				perks[i].y += perks[i].dy;
				if (perks[i].y < 0) {
					perks[i].active = false;
				}

				// Check for collision with paddle
				if (perks[i].y <= yBoard + dyBoard && perks[i].y >= yBoard &&
					perks[i].x >= xBoard && perks[i].x <= xBoard + dxBoard) {
					applyPerk(perks[i].type);
					perks[i].active = false;
				}
			}
		}

		// Check for direct hit on enemies
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemies[i].active && xBall >= enemies[i].x - 10 && xBall <= enemies[i].x + 10 &&
				yBall >= enemies[i].y - 10 && yBall <= enemies[i].y + 10) {
				dy = -dy;
				enemies[i].life--;
				if (enemies[i].life <= 0) {
					enemies[i].active = false;
					br_enemy[enemy_number++] = i;
					
				}
				if (Enemy_Count >= 0){
					Enemy_Count--;
					
					
				}
				


			}
		}

		if (all_bricks_destroyed && (Enemy_Count==0 || Enemy_Count==-1)) {
			
			if (current_level == 3) {
				in_game = false; // Reset to level 1 or end game if needed
				game_finished = true;
				relative_score = scores;
				scores = 0;
				Life = 3;
				current_level = 1;
			}
			else{
				current_level++;
				
			}

			initializeBricks();
			restartBall();
			
			
		}

		// Collision with paddle
		if (xBall >= xBoard && xBall <= xBoard + dxBoard && yBall >= yBoard && yBall <= yBoard + dyBoard) {
			double relativeHitPosition = (double)(xBall - xBoard) / dxBoard;

			if (relativeHitPosition < 0.5) { // Left half of the paddle
				qeta = 170 - 90 * relativeHitPosition * 2; // Range [90, 180] degrees
				dx = (int)(v * cos(qeta * PI / 180));
				dy = (int)(v * sin(qeta * PI / 180));
			}
			else { // Right half of the paddle
				qeta = 90 - 80 * (relativeHitPosition - 0.5) * 2; // Range [0, 90] degrees
				dx = (int)(v * cos(qeta * PI / 180));
				dy = (int)(v * sin(qeta * PI / 180));
			}

			yBall = yBoard + dyBoard + BALL_RADIUS;

		}
		else {
			if (yBall <= 0 || paddle_life <= 0) {
				restartBall();
				Life--;
				if (Life == 0) {
					score();
					relative_score = scores;
					
					scores = 0;
						
					for (int i = 0; i < brick_number; i++) {
						bar[br[i]].show = true;
					}

					for (int i = 0; i < MAX_ENEMIES; i++){
						enemies[br_enemy[i]].active = true;
					}

					Life = 3;
					brick_number = 0;
					enemy_number = 0;
					in_game = false;
					game_over = true;
					

					

					
				}
			}
		}
	}
	
}

void restartBall(){
	iText(100, 100, "HERE", GLUT_BITMAP_TIMES_ROMAN_24);
	xBall = (int)WINDOW_WIDTH / 2;
	yBall = yAxisMargin;
	dx = 7;
	dy = 5;
	xBoard = xBall - (dxBoard / 2);
	yBoard = yBall - 20;
	paddle_life = PADDLE_INITIAL_LIFE; // Reset paddle life

	game_started = false;
}
void obstacleFall() {
	if (game_started) { // Only start falling if the game has started
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemies[i].active && !obstacles[i].active) {
				obstacles[i].active = true;
				obstacles[i].x = enemies[i].x;
				obstacles[i].y = enemies[i].y;
			}
		}
	}
}

void moveObstacles() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (obstacles[i].active) {
			obstacles[i].y += obstacles[i].dy;

			// Check for collision with paddle
			if (obstacles[i].y <= yBoard + dyBoard && obstacles[i].y >= yBoard &&
				obstacles[i].x >= xBoard && obstacles[i].x <= xBoard + dxBoard) {
				paddle_life--;
				if (paddle_life <= 0) {
					game_started = false; // Game over
				}
				obstacles[i].active = false;
			}

			// Reset obstacle if it goes below the paddle
			if (obstacles[i].y < 0) {
				obstacles[i].active = false;
			}
		}
	}
}
void timerFunction() {
	autoShoot();
}
int main()
{
	iInitialize(WINDOW_WIDTH, WINDOW_HEIGHT, "DX BALL");
	//srand((unsigned int)time(0)); // Seed the random number generator
	srand(static_cast<unsigned int>(time(0)));
	loadImages();
	initializeBricks();
	iSetTimer(10, ballChange);
	iSetTimer(2000, obstacleFall); // Set timer for enemies to throw obstacles
	iSetTimer(50, moveObstacles); // Set timer to move obstacles
	iSetTimer(500, timerFunction); // Adjust timer for paddle shooter
	iStart();
	return 0;
}
