/* wiki: https://en.wikipedia.org/wiki/Elementary_cellular_automaton */
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SW 500
#define SH 500
#define SCALE 1
#define GW (int) (SCALE*SW)
#define GH (int) (SCALE*SH)
char grid[GW][GH] = {0};
void ImageFromRule(Image *image, int rule);

int main()
{
	printf("Size of grid: %.2f MB\n", (float) (sizeof (grid)) / 1000 / 1000);

	srandom(time(NULL));
	for (int i = 0; i < GW; i++) {
		grid[i][0] = random()%2;
	}
	int rule = 110;
	bool update = true;
	char buf[1024];
	Image screen = GenImageColor(GW, GH, BLACK);
	Texture2D texture;


	InitWindow(SW, SH, "Elementary Cellular Automaton");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		/* Update */
		if (IsKeyPressed(KEY_SPACE)) update = !update;
		if (IsKeyPressed(KEY_P) || IsKeyPressedRepeat(KEY_P)) {
			rule += 1 - 2 * IsKeyDown(KEY_LEFT_SHIFT);
			update = true;
		}
		if (IsKeyPressed(KEY_R)) {
			for (int i = 0; i < GW; i++) {
				grid[i][0] = random()%2;
			}
			update = true;
		}

		if (update) {
			ImageFromRule(&screen, rule);
			UnloadTexture(texture);
			texture = LoadTextureFromImage(screen);
			if (SCALE >= 1)
				SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
			update = false;
		}

		/* Draw */
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexturePro(texture, (Rectangle) {0, 0, GW, GH}, (Rectangle) {0, 0, SW, SH},
			Vector2Zero(), 0, WHITE);
		snprintf(buf, 1024, "Rule %d", rule);
		DrawFPS(0, 0);
		DrawText(buf, 0, 16, 32, RED);
		EndDrawing();

	}
	CloseWindow();

	return 0;
}

void ImageFromRule(Image *image, int rule)
{
	for (int j = 0; j < GH-1; j++) {
		for (int i = 1; i < GW-1; i++) {
			int sum = 4*grid[i-1][j] + 2*grid[i][j] + grid[i+1][j];
			int val = (rule >> sum) & 1;
			grid[i][j+1] = val;
			ImageDrawPixel(image, i, j+1, val ? WHITE : BLACK);
		}
	}
}
