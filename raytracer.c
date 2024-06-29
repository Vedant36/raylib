/*
 * Not really a raytracing implementation as it uses a hack to draw the dark
 * side instead of calculating closest distance to obstacles. But this could be
 * used as a bitmask to render scenes with light sources. Note that there is no
 * simple way to extend this to add normal or specular reflections. We'll need
 * to go back to casting rays for that.
 */
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SW 1920
#define SH 1080
#define MAXDIM SW>SH?SW:SH

typedef struct Line {
	Vector2 start, end;
} Line;
#define NUM_LINES 10
Line lines[NUM_LINES];
Vector2 tempFan[4];

void init()
{
	for (int i = 0; i < NUM_LINES; i++) {
		lines[i].start.x = random()%SW;
		lines[i].start.y = random()%SH;
		lines[i].end.x = random()%SW;
		lines[i].end.y = random()%SH;
	}
}

int main()
{
	float T = 0.3;
	Vector2 player = {SW/2, SH/2};
	InitWindow(SW, SH, "2D Raytracer");
	ToggleFullscreen();
	ToggleFullscreen();

	srandom(time(NULL));

	init();

	float deltaP = 400;
	bool isLit = 0;
	while (!WindowShouldClose())
	{
		Vector2 mouse = GetMousePosition();
		float delta = GetFrameTime();
		;
		float x = mouse.x, y = mouse.y;
		/* Update */
		if (IsKeyPressed(KEY_R)) init();
		if (IsKeyDown(KEY_LEFT))  player.x -= deltaP * delta;
		if (IsKeyDown(KEY_RIGHT)) player.x += deltaP * delta;
		if (IsKeyDown(KEY_UP))    player.y -= deltaP * delta;
		if (IsKeyDown(KEY_DOWN))  player.y += deltaP * delta;
		if (IsKeyPressed(KEY_L)) isLit = !isLit;
		// calculate rays

		/* Draw */
		BeginDrawing();
		char buf[64];
		snprintf(buf, 64, "Transmission coefficient: %.2f", T);
		DrawFPS(0, 0);
		DrawText(buf, 0, 16, 16, GREEN);
		DrawCircleV(player, 2, RED);
		if (isLit) {
			ClearBackground(WHITE);
			for (int i = 0; i < NUM_LINES; i++) {
				// Draw twice because DrawTriangleStrip needs the right orientation
				tempFan[0] = lines[i].start;
				tempFan[1] = lines[i].end;
				tempFan[2] = Vector2Add(player, Vector2Scale(Vector2Subtract(lines[i].start, player), MAXDIM));
				tempFan[3] = Vector2Add(player, Vector2Scale(Vector2Subtract(lines[i].end, player), MAXDIM));
				DrawTriangleStrip(tempFan, 4, Fade(BLACK, 1-T));
				tempFan[1] = lines[i].start;
				tempFan[0] = lines[i].end;
				tempFan[3] = Vector2Add(player, Vector2Scale(Vector2Subtract(lines[i].start, player), MAXDIM));
				tempFan[2] = Vector2Add(player, Vector2Scale(Vector2Subtract(lines[i].end, player), MAXDIM));
				DrawTriangleStrip(tempFan, 4, Fade(BLACK, 1-T));
			}
			/* DrawCircleV(mouse, 3, GOLD); */
			DrawFPS(0, 0);
		} else {
			ClearBackground(BLACK);
			for (int i = 0; i < NUM_LINES; i++) {
				DrawLineV(lines[i].start, lines[i].end, GOLD);
			}
			DrawFPS(0, 0);
		}

		EndDrawing();
	}
	CloseWindow();

	return 0;
}

