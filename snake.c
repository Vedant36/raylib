/*
 * TODO
 * - invert SnakeMove
 * - add food
 * - add death
 * - game screen
 */

#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SW 500
#define SH 500
#define TICK 0.1

#define CW 20
#define CH 20
/* cell size and margins */
const int CS = (SW/CW<SH/CH?SW/CW:SH/CH),
	CXM = (SW/CW<SH/CH?0:SW-CS*CW)/2,
	CYM = (SH/CH<SW/CW?0:SH-CS*CH)/2;


#define SNAKE_LENGTH 5
typedef struct Snake {
	int x, y;
	struct Snake *tail;
} Snake;

enum SnakeDirection {LEFT, UP, RIGHT, DOWN};
enum SnakeMoving {PAUSED, MOVING, DEAD};
void SnakeAppend(Snake *snake, int x, int y);
void SnakeMove(Snake *snake, int dx, int dy);
void SnakeDraw(Snake *snake, Color color);
void SnakeFree(Snake *snake);
void SnakeDump(Snake *snake);
Snake *SnakeHeadCollide(Snake *snake, Snake head);

Snake SpawnFood(Snake *snake);

int main()
{
	srandom(time(NULL));

	Snake snake = {.x = 0, .y = 0, .tail = NULL};
	for (int i = 1; i < SNAKE_LENGTH; i++)
		SnakeAppend(&snake, i, 0);
	enum SnakeDirection dir = RIGHT;
	enum SnakeMoving mov = PAUSED;
	Snake food = SpawnFood(&snake);

	float tick = 0;
	printf("%d %d %d\n", CS, CXM, CYM);

	InitWindow(SW, SH, "Snake Game");
	ToggleFullscreen();
	ToggleFullscreen();
	SetTargetFPS(60);
	SnakeDump(&snake);
	SnakeDump(&food);

	while (!WindowShouldClose())
	{
		/* Update */
		// Change direction
		if (IsKeyPressed(KEY_R)) {
			SnakeFree(snake.tail);
			snake.x = 0;
			snake.y = 0;
			snake.tail = NULL;
			for (int i = 1; i < SNAKE_LENGTH; i++)
				SnakeAppend(&snake, i, 0);
			dir = RIGHT;
			mov = PAUSED;
			SnakeDump(&snake);
			printf("mov: %d\n", mov);
			food = SpawnFood(&snake);
		}
		if (IsKeyPressed(KEY_SPACE)) {
			if (mov == MOVING) mov = PAUSED;
			else if (mov == PAUSED) mov = MOVING;
			printf("mov: %d\n", mov);
		}
		if (mov == MOVING) {
			tick += GetFrameTime();
			if (IsKeyPressed(KEY_LEFT) && dir != RIGHT) dir = LEFT;
			else if (IsKeyPressed(KEY_RIGHT) && dir != LEFT) dir = RIGHT;
			else if (IsKeyPressed(KEY_UP) && dir != DOWN) dir = UP;
			else if (IsKeyPressed(KEY_DOWN) && dir != UP) dir = DOWN;
		}
		if (mov != DEAD && SnakeHeadCollide(snake.tail, snake)) {
			printf("dead\n");
			SnakeDump(&snake);
			mov = DEAD;
		}
		if (mov == MOVING && snake.x == food.x && snake.y == food.y) {
			SnakeAppend(&snake, food.x, food.y);
			food = SpawnFood(&snake);
		}

		if (tick > TICK && mov == MOVING) {
			tick -= TICK;
			switch (dir) {
			case LEFT:
				SnakeMove(&snake, -1, 0);
				break;
			case UP:
				SnakeMove(&snake, 0, -1);
				break;
			case RIGHT:
				SnakeMove(&snake, 1, 0);
				break;
			case DOWN:
				SnakeMove(&snake, 0, 1);
				break;
			}
		}

		/* Draw */
		BeginDrawing();
		ClearBackground(BLACK);
		// Draw grid, food, snake
		for (int i = 0; i < CW; i++) {
			for (int j = 0; j < CH; j++) {
				if ((i+j)%2)
					DrawRectangle(CXM+i*CS, CYM+j*CS, CS, CS, (Color) {7, 7, 7, 255});
				else
					DrawRectangle(CXM+i*CS, CYM+j*CS, CS, CS, (Color) {15, 15, 15, 255});
			}
		}
		// snake
		SnakeDraw(&food, GOLD);
		SnakeDraw(&snake, GREEN);
		if (mov == DEAD) {
			DrawText("DEAD", SW/2-85, SH/2-60, 64, RED);
			DrawText("Press R to restart", SW/2-160, SH/2, 32, RED);
		}
		EndDrawing();
	}
	CloseWindow();
	SnakeFree(snake.tail);

	return 0;
}

void SnakeAppend(Snake *snake, int x, int y)
{
	while (snake->tail != NULL)
		snake = snake->tail;
	snake->tail = malloc(sizeof (Snake));
	*(snake->tail) = (Snake) {x, y, NULL};
	printf("Allocated snake %d,%d\n", snake->tail->x, snake->tail->y);
}

void SnakeFree(Snake *snake)
{
	Snake *tail;
	while (snake != NULL) {
		tail = snake->tail;
		printf("Freed snake %d,%d\n", snake->x, snake->y);
		free(snake);
		snake = tail;
	}
}

void SnakeMove(Snake *snake, int dx, int dy)
{
	Snake *head = snake;
	while (head->tail != NULL) {
		head->x = head->tail->x;
		head->y = head->tail->y;
		head = head->tail;
	}
	head->x += dx;
	head->y += dy;
}

void SnakeDraw(Snake *snake, Color color)
{
	while (snake != NULL) {
		int i = snake->x, j = snake->y;
		DrawRectangle(CXM+i*CS+1, CYM+j*CS+1, CS-2, CS-2, color);
		snake = snake->tail;
	}
}

void SnakeDump(Snake *snake)
{
	printf("Snake: [");
	while (snake != NULL) {
		int i = snake->x, j = snake->y;
		printf("(%d,%d)", i, j);
		snake = snake->tail;
	}
	printf("]\n");
}

Snake *SnakeHeadCollide(Snake *snake, Snake head)
{

	while (snake->tail != NULL) {
		if (snake->x == head.x && snake->y == head.y) {
			printf("match: %d %d\n", snake->x, snake->y);
			return snake;
		}
		snake = snake->tail;
	}
	return NULL;
}

Snake SpawnFood(Snake *snake)
{
	Snake food;
	food.tail = NULL;
	do {
		food.x = random()%CW;
		food.y = random()%CH;
	} while (SnakeHeadCollide(snake, food));
	return food;
}
