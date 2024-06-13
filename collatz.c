#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define SW 500
#define SH 500

#define C(n) (n<0?0:n>255?255:n)

void CollatzDraw(int num, int eoff, int rec, Color c);

int main()
{
	srandom(time(NULL));
	int num = 27;
	int len = 10;
	InitWindow(SW, SH, "Collatz Conjecture");
	ToggleFullscreen();
	ToggleFullscreen();
	/* SetTargetFPS(60); */

	while (!WindowShouldClose())
	{
		/* Update */
		;

		/* Draw */
		BeginDrawing();
		DrawFPS(0, 0);
		ClearBackground(BLACK);
		CollatzDraw(1, 0, 0, WHITE);
		EndDrawing();
	}
	CloseWindow();

	return 0;
}

void CollatzDraw(int num, int eoff, int rec, Color c)
{
	if (rec > 50 || num > SH) return;
	srandom(num);
	int _eoff = eoff + 10;
	Color cur = {
		.r=C(c.r+(random()%20-10)),
		.b=C(c.b+(random()%20-10)),
		.g=C(c.g+(random()%20-10)),
		.a=127};
	DrawLine(rec*10, num, _eoff, 2*num, cur);
	CollatzDraw(2*num, _eoff, rec+1, cur);

	if (num%3 == 1) {
		DrawLine(rec*10, num, _eoff, (num-1)/3, cur);
		CollatzDraw((num-1)/3, _eoff, rec+1, cur);
	}
}
