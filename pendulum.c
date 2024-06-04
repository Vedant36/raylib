#include <raylib.h>
#include <raymath.h>
#include <math.h>

#define SW 500
#define SH 500

#define G 400

int main()
{
	InitWindow(SW, SH, "Pendulum");
	ToggleFullscreen();
	ToggleFullscreen();
	SetTargetFPS(60);

	Vector3 theta = Vector3Zero();
	theta.x = M_PI/3;
	float stringLength = SH/2;

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		/* Forces */
		theta.z = - (G/stringLength) * sinf(theta.x);

		/* Update */
		if (IsKeyPressed(KEY_R)) {
			theta = Vector3Zero();
			theta.x = M_PI/3;
			BeginDrawing();
			ClearBackground(BLACK);
			EndDrawing();
		}
		if (IsKeyPressed(KEY_SPACE) || 1) {
			theta.y += theta.z * deltaTime;
			theta.x += theta.y * deltaTime;
		}

		/* Draw */
		BeginDrawing();
		DrawFPS(0, 0);
		ClearBackground(BLACK);
		Vector2 pos = {
			SW/2 + stringLength * sinf(theta.x),
			stringLength * cosf(theta.x)
		};
		DrawLine(SW/2, 0, pos.x, pos.y, WHITE);
		DrawCircleV(pos, 5, WHITE);
		EndDrawing();
	}
	CloseWindow();

	return 0;
}
