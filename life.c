#include <stdio.h>
#include <raylib.h>

#define SW 1024
#define SH 576
#define SIZE 16
#define CW (SW/SIZE)
#define CH (SH/SIZE)

int cells[CW][CH] = {0};
int next[CW][CH] = {0};

int main()
{
	Vector2 mouseP;
	InitWindow(SW, SH, "Game of Life");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		/* Update */
		if (IsKeyPressed(KEY_R)) {
			for (int i = 0; i < CW; i++)
				for (int j = 0; j < CH; j++)
					cells[i][j] = 0;
		}
		if (IsKeyPressed(KEY_S) || IsKeyPressedRepeat(KEY_S)) {
			for (int i = 0; i < CW; i++) {
				for (int j = 0; j < CH; j++) {
					// torus
					int a=(i-1+CW)%CW, b=(i+1)%CW, c=(j-1+CH)%CH, d=(j+1)%CH;
					int sum = cells[a][c] + cells[i][c] + cells[b][c]
						+ cells[a][j] + 0           + cells[b][j]
						+ cells[a][d] + cells[i][d] + cells[b][d];
					next[i][j] = 0;
					if (cells[i][j] == 0 && sum == 3)
						next[i][j] = 1;
					else if (cells[i][j] == 1 && (sum == 2 || sum == 3))
						next[i][j] = 1;
				}
			}
			for (int i = 0; i < CW; i++)
				for (int j = 0; j < CH; j++)
					cells[i][j] = next[i][j];
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			mouseP = GetMousePosition();
			int x = (int)(mouseP.x/16), y = (int)(mouseP.y/16);
			cells[x][y] = !cells[x][y];
		}

		/* Draw */
		BeginDrawing();
		ClearBackground(BLACK);
		for (int i = 0; i < CW; i++) {
			for (int j = 0; j < CH; j++) {
				if (cells[i][j] == 1)
					DrawRectangle(i*16, j*16, 16, 16, (Color){240, 240, 240, 255});
				else if ((i+j)%2==0)
					DrawRectangle(i*16, j*16, 16, 16, (Color){30, 30, 30, 255});
				else
					DrawRectangle(i*16, j*16, 16, 16, (Color){27, 27, 27, 255});
			}
		}
		EndDrawing();
	}
	CloseWindow();

	return 0;
}
