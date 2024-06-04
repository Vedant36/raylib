#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>
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
	InitWindow(SW, SH, "2D Raytracer");
	ToggleFullscreen();
	ToggleFullscreen();

	srandom(time(NULL));

	init();

	while (!WindowShouldClose())
	{
		Vector2 mouse = GetMousePosition();
		float x = mouse.x, y = mouse.y;
		/* Update */
		if (IsKeyPressed(KEY_R)) init();
		// calculate rays

		/* Draw */
		BeginDrawing();
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			ClearBackground(WHITE);
			for (int i = 0; i < NUM_LINES; i++) {
				// Draw twice because DrawTriangleStrip needs the right orientation
				tempFan[0] = lines[i].start;
				tempFan[1] = lines[i].end;
				tempFan[2] = Vector2Add(mouse, Vector2Scale(Vector2Subtract(lines[i].start, mouse), MAXDIM));
				tempFan[3] = Vector2Add(mouse, Vector2Scale(Vector2Subtract(lines[i].end, mouse), MAXDIM));
				DrawTriangleStrip(tempFan, 4, (Color) {0, 0, 0, 240});
				tempFan[1] = lines[i].start;
				tempFan[0] = lines[i].end;
				tempFan[3] = Vector2Add(mouse, Vector2Scale(Vector2Subtract(lines[i].start, mouse), MAXDIM));
				tempFan[2] = Vector2Add(mouse, Vector2Scale(Vector2Subtract(lines[i].end, mouse), MAXDIM));
				DrawTriangleStrip(tempFan, 4, (Color) {0, 0, 0, 240});
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

