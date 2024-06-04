#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>

#define SW 500
#define SH 500
#define SUB 2

#define ITER 30
const float XOFF = (2*SW*SUB/3),
	YOFF = (SH*SUB/2);
Color screen[SW*SUB][SH*SUB];

void UpdateMandelbrot(Vector2 offset);

Color ColorBlend(Color c1, Color c2, float t)
{
	return (Color) {c1.r+t*(c2.r-c1.r), c1.g+t*(c2.g-c1.g),
		c1.b+t*(c2.b-c1.b), c1.a+t*(c2.a-c1.a)};
}

Color FractionToColor(float t)
{
	/* return ColorBlend(BLACK, WHITE, 1 - t); */
	float a = 0.2, b = 0.4, c = 0.6;
	if (t < a)
	        return ColorBlend(BLACK, WHITE, t/a);
	else if (t < b)
	        return ColorBlend(WHITE, ORANGE, (t-a)/(b-a));
	else if (t < c)
	        return ColorBlend(ORANGE, BLUE, sqrtf((t-b)/(c-b)));
	else
	        return ColorBlend(BLUE, BLACK, pow((t-c)/(1-c), 1));
}

int main()
{
	Vector2 offset = {0};
	int update = true;

	InitWindow(SW, SH, "Mandelbrot");
	ToggleFullscreen();
	ToggleFullscreen();
	/* SetTargetFPS(60); */

	while (!WindowShouldClose()) {
		/* if (IsKeyPressed(KEY_R)) { */
		/* 	offset.x = 0; */
		/* 	offset.y = 0; */
		/* 	update = true; */
		/* } */
		/* if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { */
		/* 	Vector2 mouse = GetMouseDelta(); */
		/* 	offset = Vector2Add(offset, mouse); */
		/* 	update = true; */
		/* 	printf("%f %f;", offset.x, offset.y); */
		/* } */
		if (update) {
			UpdateMandelbrot(offset);
			update = false;
		}

		BeginDrawing();
		ClearBackground(WHITE);
		for (int i = 0; i < SW*SUB; i++) {
			for (int j = 0; j < SH*SUB; j++) {
				DrawPixel(i/SUB, j/SUB, Fade(screen[i][j], 1./(float)SUB));
			}
		}
		DrawFPS(0, 0);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void UpdateMandelbrot(Vector2 offset)
{
	for (int i = 0; i < SW*SUB; i++) {
		for (int j = 0; j < SH*SUB; j++) {
			Vector2 z = {0, 0}, p = {3.*((float)i - XOFF)/(SH*SUB), 3.*((float)j - YOFF)/(SH*SUB)};
			float k;
			float t;
			for (k = 0; k < ITER; k++) {
				float tmp = z.x*z.x - z.y*z.y + p.x + offset.x;
				z.y = 2*z.x*z.y + p.y + offset.y;
				z.x = tmp;
				t = Vector2Length(Vector2Subtract(z, p));
				if (t > 5) break;
			}
			screen[i][j] = FractionToColor((float)(ITER - k)/(float)ITER);
		}
		if ((i+1)%100 == 0)
			printf("%d Done\n", i+1);
	}
}
