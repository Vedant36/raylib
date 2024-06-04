#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define CLAMP(v, a, b) (v >= a ? (v <= b ? v : b) : a)

#define SW 640
#define SH 512

#define CW 20
#define CH 10
int cells[CW][CH];

#define SCALE SW/(CW+2)

Vector2 CollideNormal(Rectangle rec1, Rectangle rec2);

int main()
{
	printf("Scale: %d", SCALE);

	const Rectangle screen = {0, 0, SW, SH};
	srandom(45);
	for (int i = 0; i < CW; i++) {
		for (int j = 0; j < CH; j++) {
			cells[i][j] = random()%5 > 1;
		}
	}

	const int paddleWidth = 128;
	Rectangle paddle = {SW/2 - paddleWidth/2, SH - 80, paddleWidth, 16};
	int paddleSpeed = 10;

	const int ballR = 8;
	Vector2 ballP = {paddle.x+paddle.width/2, paddle.y-ballR};
	Vector2 ballV = {0, 0};
	int gameStart = 0;
	InitWindow(SW, SH, "Brick Breaker");
	SetTargetFPS(60);


	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		if (IsKeyPressed(KEY_SPACE) && !gameStart) {
			ballV.y = -10;
			ballV.x = -10;
			gameStart = 1;
			printf("Game started");
		}
		if (IsKeyPressed(KEY_R)) {
			paddle.x = SW/2 - paddleWidth/2;
		}
		if (IsKeyDown(KEY_LEFT))
			paddle.x = paddle.x - paddleSpeed >= 0 ? paddle.x - paddleSpeed : 0;
		if (IsKeyDown(KEY_RIGHT))
			paddle.x = paddle.x + paddleSpeed >= 0 ? paddle.x + paddleSpeed : 0;

		// Physics
		if (gameStart) {
			ballP = Vector2Add(ballP, ballV);
		}
		// Wall collision
		if ((Vector2 normal = CollideNormal((Rectangle){ballP.x-ballR, ballP.y-ballR, 2*ballR, 2*ballR}, screen)))
			Vector2Subtract(ballV, Vector2Scale(ballV, 2 * Vector2DotProduct(ballV, normal)));

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangleRec(paddle, GOLD);
		DrawCircleV(ballP, ballR, RED);
		// draw the grid
		for (int i = 0; i < CW; i++) {
			for (int j = 0; j < CH; j++) {
				if (cells[i][j] == 1)
					DrawRectangle((i+1)*SCALE+1, 32 + j*16+1, SCALE-2, 14, (Color){240, 240, 240, 255});
			}
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context

	return 0;
}

Vector2 ContainRec(Rectangle rec1, Rectangle rec2)
{
	return (rec1.x <= rec2.x && rec1.y <= rec2.y
		&& rec1.x+rec1.width >= rec2.x+rec2.width
		&& rec1.y+rec1.height >= rec2.y+rec2.height);
}

Vector2 CollideNormal(Rectangle rec1, Rectangle rec2)
{
	if (!CheckCollisionRecs(rec1, rec2)) return NULL;
	Vector2 normal;
	return normal;
}
