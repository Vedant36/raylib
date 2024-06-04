/* A Double Pendulum */
#include <raylib.h>
#include <math.h>

#define SW 500
#define SH 500

#define G 400

int main()
{
	InitWindow(SW, SH, "Double Pendulum without the cross term");
	ToggleFullscreen();
	ToggleFullscreen();
	/* SetTargetFPS(60); */

	Vector3 t1 = {M_PI/2, M_PI, 0}, t2 = {M_PI/3, M_PI, 0};
	float sl = SH/5;
	Vector2 center = {SH/2, SW/2};

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		/* Forces */
		t1.z = - 2 * (G/sl) * sinf(t1.x);
		t2.z = - (G/sl) * sinf(t2.x);

		/* Update */
		if (IsKeyPressed(KEY_SPACE) || 1) {
			t1.y += t1.z * deltaTime;
			t1.x += t1.y * deltaTime;
			t2.y += t2.z * deltaTime;
			t2.x += t2.y * deltaTime;
		}

		/* Draw */
		BeginDrawing();
		/* DrawFPS(0, 0); */
		ClearBackground(BLACK);
		Vector2 p1 = {
			center.x + sl * sinf(t1.x),
			center.y + sl * cosf(t1.x)
		};
		DrawLine(center.x, center.y, p1.x, p1.y, GOLD);
		DrawCircleV(p1, 2, GOLD);
		/* DrawPixelV(p1, GOLD); */
		Vector2 p2 = {
			sl * sinf(t2.x),
			sl * cosf(t2.x)
		};
		DrawLine(p1.x, p1.y, p1.x+p2.x, p1.y+p2.y, WHITE);
		DrawCircle(p1.x+p2.x, p1.y+p2.y, 2, WHITE);
		/* DrawPixelV((Vector2){p1.x+p2.x, p1.y+p2.y}, (Color){255,255,255,32}); */
		EndDrawing();
	}
	CloseWindow();

	return 0;
}

