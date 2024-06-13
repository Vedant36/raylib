/* Scroll to zoom, click mouse to move around*/
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>

#define SW 1247
#define SH 1080

void SerpinskiDraw(Rectangle screen, float rec);
float rec = 1;
Rectangle camera = {0, 0, SW, SH};


float main()
{
	double zoom = 1, zfact = 0.1;
	Rectangle screen = {0, 0, SW, SH};
	InitWindow(SW, SH, "Serpinksi's Triangle");
	ToggleFullscreen();
	ToggleFullscreen();
	SetTargetFPS(60);
	Vector2 mouse, mouselast;

	while (!WindowShouldClose())
	{
		/* Update */
		if (IsKeyPressed(KEY_UP)) rec++;
		if (IsKeyPressed(KEY_DOWN)) rec--;
		if (IsKeyPressed(KEY_R)) {
			zoom = 1;
			screen.x = camera.x;
			screen.y = camera.y;
			screen.width = camera.width;
			screen.height = camera.height;
		}

		float wheel = GetMouseWheelMove();
		if (wheel) {
			mouse = GetMousePosition();
			zoom *= 1 + zfact*wheel;
			if (zoom < 0) zoom = 0;
			printf("Zoom: %f, rec: %f\n", zoom, rec);
			Vector2 tmp;
			tmp.x = (mouse.x - screen.x)/screen.width;
			tmp.y = (mouse.y - screen.y)/screen.height;
			screen.width  = camera.width * zoom;
			screen.height = camera.height * zoom;
			screen.x = mouse.x - tmp.x*screen.width;
			screen.y = mouse.y - tmp.y*screen.height;
		}
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			mouse = GetMouseDelta();
			screen.x += mouse.x;
			screen.y += mouse.y;
		}

		/* Draw */
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawFPS(0, 0);
		SerpinskiDraw(screen, rec);
		EndDrawing();
	}
	CloseWindow();

	return 0;
}

void SerpinskiDraw(Rectangle screen, float rec)
{
	if (rec < 0) return;
	if (!CheckCollisionRecs(screen, camera)) return;
	float x=screen.x, y=screen.y, w=screen.width, h=screen.height;
	DrawTriangleLines((Vector2){x+w/2,y},
		(Vector2){x,y+h},
			(Vector2){x+w,y+h}, Fade(RED, 0.3));
	SerpinskiDraw((Rectangle) {x, y+h/2, w/2, h/2}    , rec-1);
	SerpinskiDraw((Rectangle) {x+w/2, y+h/2, w/2, h/2}, rec-1);
	SerpinskiDraw((Rectangle) {x+w/4, y, w/2, h/2}    , rec-1);
}
