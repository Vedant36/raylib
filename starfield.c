#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SW 500
#define SH 500

#define NUM_STARS 1000
float d = 100, zinit = SW/2, vel = 1;
typedef struct Star {
	float x, y, z;
} Star;
Star stars[NUM_STARS];

void StarInit(Star *star)
{
	star->x = random()%((int) (SW*zinit/d)) - SW*zinit/d/2;
	star->y = random()%((int) (SH*zinit/d)) - SH*zinit/d/2;
	star->z = zinit;
}

Vector2 StarCoordTransform(Star star)
{
	float x = star.x, y = star.y, z = star.z;
	return (Vector2) {SW/2 + x*d/z, SH/2 + y*d/z};
}

void StarUpdate(Star *star, float deltaTime)
{
	Vector2 temp = StarCoordTransform(*star);
	star->z -= vel * deltaTime;
	if (0 > temp.x || temp.x > SW || 0 > temp.y || temp.y > SH) {
		StarInit(star);
	}
}

void StarDraw(Star star)
{
	Vector2 temp = StarCoordTransform(star);
	DrawPixelV(temp, WHITE);
}

int main()
{
	int drawing = 1;
	srandom(time(NULL));
	for (int i = 0; i < NUM_STARS; i++)
		StarInit(&stars[i]);

	InitWindow(SW, SH, "starfield");
	ToggleFullscreen();
	ToggleFullscreen();
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		vel += GetMouseDelta().x;
		/* Update */
		if (IsKeyPressed(KEY_SPACE)) drawing = !drawing;
		if (IsKeyPressed(KEY_R)) {
			for (int i = 0; i < NUM_STARS; i++)
				StarInit(&stars[i]);
		}

		for (int i = 0; i < NUM_STARS; i++)
			StarUpdate(&stars[i], deltaTime);

		/* Draw */
		BeginDrawing();
		if (drawing) {
			ClearBackground(Fade(BLACK, 0.2));
			for (int i = 0; i < NUM_STARS; i++)
				StarDraw(stars[i]);
		}
		EndDrawing();
	}
	CloseWindow();

	return 0;
}

