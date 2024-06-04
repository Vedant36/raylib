/*
  TODO
  - Better Bird_Distance to account for wrapping
  - Quad mesh
  - Object avoidance
  - Goal seeking
  - Readable code
  - 3D
 */

#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* #define DEBUG */

#define L(v) Vector2Length(v)
#define A(v1, v2) Vector2Add(v1, v2)
#define S(v, k) Vector2Scale(v, k)
#define V0() Vector2Zero()
#define BIND(key, param) if (IsKeyPressed(KEY_##key) || IsKeyPressedRepeat(KEY_##key)) param += 1 - 2 * IsKeyDown(KEY_LEFT_SHIFT)

#define SW 500
#define SH 500

#define BC 1000                                        // Bird count
#define BV 40                                          // Max bird velocity
#define BA 10                                          // Max bird acceleration
#define BIRD_COLOR CLITERAL(Color){ 200, 160, 0, 127 } // Default bird color
typedef struct Bird {
	Vector2 p, v, a;
	Color color;
} Bird;
Bird birds[BC];
float p0=1, p1=1, p2=1;
int p3=100;

void *CalculateAccel(void *i);
int ColorDist(Color c1, Color c2);
void init();
Vector2 Vector2Wrap(Vector2 v1, Vector2 min, Vector2 max);
void BirdLog(Bird bird);

int main()
{
	int isRunning=1, viewMode=0, colorMode=0;
	char s[64];
	int loopCount;
	init();

	Vector2 mouseP;
	InitWindow(SW, SH, "Boid Simulation");
	ToggleBorderlessWindowed();
	ToggleFullscreen();
	ToggleFullscreen();
	/* SetTargetFPS(60); */

	while (!WindowShouldClose())
	{
		loopCount = 0;
		float deltaTime = GetFrameTime();
		/* Logic: whole section only to get birds[*].a */

		for (int i = 0; i < BC; i++) {
			Vector2 p=birds[i].p, v=birds[i].v;
			Vector2 sep = Vector2Zero();
			Vector2 tv = Vector2Zero();
			Vector2 tp = Vector2Zero();
			int c = 0;
			for (int j = 0; j < BC && i!=j; j++) {
				float dist = Vector2Distance(birds[i].p, birds[j].p);
				if (dist < p3) {
					float vdist = Vector2Distance(birds[i].v, birds[j].v);
					/* sep = A(sep, S(Vector2Subtract(p, birds[j].p), */
							/* -pow(Vector2Distance(p, birds[j].p) - 1, 2))); */
					sep = Vector2Add(sep, Vector2Scale(Vector2Subtract(p, birds[j].p), 1./pow(dist, 3)));
					tp = Vector2Add(tp, birds[j].p);
					tv = Vector2Add(tv, birds[j].v);
					c++;
					loopCount++;
				}
			}
			if (c) {
				Vector2 dv=Vector2Scale(tv,1.f/c),
					dp=Vector2Subtract(Vector2Scale(tp,1.f/c), p);
				birds[i].a = Vector2ClampValue(Vector2Add(Vector2Scale(sep, p0),
							Vector2Add(Vector2Scale(dv, p1),
								Vector2Scale(dp, p2))),
							0, BA);
			} else {
				birds[i].a = sep;
			}
		}
		printf("%d\n", loopCount);

		/* Update: 0.001s or 1000fps */
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mouse = GetMousePosition();
			for (int i = 0; i < BC; i++) {
				if (Vector2Distance(birds[i].p, mouse) < 20) {
					BirdLog(birds[i]);
					printf("%f,%f;%f,%f;%f,%f\n", birds[i].p.x, birds[i].p.y,
						birds[i].v.x, birds[i].v.y,
						birds[i].a.x, birds[i].a.y);
					fflush(stdout);
				}
			}
			printf("\n");
		}
		if (IsKeyPressed(KEY_R)) init();
		BIND(SPACE, isRunning);
		BIND(U, p0);
		BIND(I, p1);
		BIND(O, p2);
		BIND(P, p3);
		BIND(V, viewMode);

		if (isRunning%2) {
			for (int i = 0; i < BC; i++) {
				birds[i].v = Vector2ClampValue(A(birds[i].v, S(birds[i].a, deltaTime)), 0, BV);
				birds[i].p = Vector2Wrap(A(birds[i].p, S(birds[i].v, deltaTime)),
							Vector2Zero(), (Vector2){SW, SH});
			}
		}

		/* Draw */
		BeginDrawing();
		ClearBackground(BLACK);
		// loop time: 0.0023s or 300fps
		for (int i = 0; i < BC; i++) {
			Vector2 p=birds[i].p;
			DrawPixelV(p, BIRD_COLOR);
		}
		if (viewMode%2 == 1) {
			DrawFPS(0, 0);
			snprintf(s, 64, "%f, %f, %f, %d, %d", p0, p1, p2, p3);
			DrawText(s, 0, 16, 16, WHITE);
		}
		EndDrawing();
	}
	CloseWindow();

	return 0;
}

void *CalculateAccel(void *ip)
{
	int i = *(int*)ip;
}

int ColorDist(Color c1, Color c2)
{
	return sqrt(pow(c1.r - c2.r, 2)
		+pow(c1.g - c2.g, 2)
		+pow(c1.b - c2.b, 2));
}

void init()
{
	srand(time(NULL));
	/* randomize bird positions and velocities */
	for (int i = 0; i < BC; i++) {
		birds[i].p.x = random()%SW;
		birds[i].p.y = random()%SH;
		birds[i].v.x = random()%BV - BV/2;
		birds[i].v.y = random()%BV - BV/2;
		birds[i].a = Vector2Zero();
		birds[i].color = ColorFromHSV(random()%360, 128+random()%112, 128+random()%112);
	}

}

// Wrap input vector from min to max
Vector2 Vector2Wrap(Vector2 v, Vector2 min, Vector2 max)
{
	Vector2 result = { 0 };

	result.x = Wrap(v.x, min.x, max.x);
	result.y = Wrap(v.y, min.y, max.y);

	return result;
}

void BirdLog(Bird bird)
{
	printf("%f,%f;%f,%f;%f,%f\n", bird.p.x, bird.p.y,
		bird.v.x, bird.v.y,
		bird.a.x, bird.a.y);
}
