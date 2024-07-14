#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SW 500
#define SH 500
#define BGCOLOR ((Color) {13, 14, 13, 255})
#define FGCOLOR ((Color) {23, 24, 23, 255})
#define MAX_INPUT_CHARS 2048
#define R 2
typedef unsigned int uint;
typedef struct huffman_tree {
	uint node;
	struct huffman_tree *l, *r;
} htree;
void htree_print(htree *h);
#define VAL(a) (a<256 ? counts[a] : treelist[a-256] ? treelist[a-256]->node : 0)
int htree_depth(htree *h);
int htree_count(htree *h);
htree *htree_generate(char *buf);
void DrawHuffmanTree(htree *h, Rectangle s, Vector2 prev);
void htree_free(htree *h);

int main()
{
	/* Source: JC McKeown - Classical Latin */
	char buf[] = "despite the fact that english is a germanic language, you probably found it much easier to guess at the meaning of the latin version. in the same way, throughout this book, you will be able to use your knowledge of english to identify the meaning of many latin words. this latinate aspect of english will also make it easier for you to remember the latin vocabulary once you have studied it.";
	/* printf("Buffer: %u bytes\n%s\n----\n", strlen(buf), buf); */
	/* htree *h = htree_generate(buf); */

	/* printf("The Huffman tree:\n"); */
	/* htree_print(h); */
	/* putchar(10); */
	/* printf("Depth of the Huffman tree: %d", htree_depth(h)); */

	InitWindow(SW, SH, "Huffman Encoding demo");
	SetTargetFPS(60);
	uint pad = 30;
	Rectangle textbox = {pad, pad, (SW-2*pad), 3*(SH-3*pad)/8};
	bool textactive = true;
	char textbuffer[MAX_INPUT_CHARS+1] = "something to pad runtime";
	int textlen = strlen(textbuffer);
	Rectangle graph = {pad, 2*pad+3*(SH-2*pad)/8, (SW-2*pad), 5*(SH-3*pad)/8};
	htree *huffman;
	while (!WindowShouldClose())
	{
		/* Update */
		int key = GetCharPressed();

		// Check if more characters have been pressed on the same frame
		while (key > 0)
		{
			// NOTE: Only allow keys in range [32..125]
			if ((key >= 32) && (key <= 125) && (textlen < MAX_INPUT_CHARS))
			{
				textbuffer[textlen] = (char)key;
				textbuffer[textlen+1] = '\0'; // Add null terminator at the end of the string.
				textlen++;
			}

			key = GetCharPressed();  // Check next character in the queue
		}

		if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
		{
			textlen--;
			if (textlen < 0) textlen = 0;
			textbuffer[textlen] = '\0';
		}

		/* Draw */
		BeginDrawing();
		ClearBackground(BGCOLOR);
		DrawRectangleRec(textbox, FGCOLOR);
		DrawText(textbuffer, textbox.x, textbox.y, 12, WHITE);
		/* DrawRectangleLinesEx(graph, 1, RED); */
		if ((huffman = htree_generate(textbuffer)))
			DrawHuffmanTree(huffman, graph, (Vector2) {-1, -1});
		EndDrawing();
		if (huffman)
			htree_free(huffman);
		huffman = NULL;
		/* CloseWindow(); */
		/* return 0; */
       }
	CloseWindow();

	return 0;
}

int htree_depth(htree *h) {
	if (h == NULL) return -1;
	if (h->l == NULL && h->r == NULL) return 0;
	int l = htree_depth(h->l);
	int r = htree_depth(h->r);
	return 1 + (r>l ? r : l);
}

int htree_count(htree *h) {
	if (h == NULL) return 0;
	if (h->l == NULL && h->r == NULL) return 1;
	return htree_count(h->l) + htree_count(h->r);
}

void htree_print(htree *h)
{
	if (h == NULL) return;
	if (h->l == NULL && h->r == NULL) {
		printf("%c", h->node);
		return;
	} else {
		printf("%u:(", h->node);
	}
	htree_print(h->l);
	printf(" ");
	htree_print(h->r);
	printf(")");
}

htree *htree_generate(char *buf)
{
	if (strlen(buf) == 0)
		return NULL;
	uint counts[256] = {0}, filled = 0;
	for (uint i = 0; i < strlen(buf); i++) {
		counts[buf[i]]++;
	}
	/* for (uint i = 0; i < 256; i++) { */
	/* 	if (counts[i] > 0) { */
	/* 		printf("`%c`: %2u\t", i, counts[i]); */
	/* 		filled++; */
	/* 		if (filled%5 == 0) putchar('\n'); */
	/* 	} */
	/* } */
	/* putchar(10); */
	/* printf("Total unique characters: %u\n\n", filled); */

	htree *treelist[256/2] = {0}; /* to store temporary trees */
	int treecount = 0;
	/* setting them to -1 then checking if minx > 0 is a nice way to not have to set defaults */
	while (true) {
		int min1 = -1, min2 = -1;
		for (int j = 0; j < 256 + treecount; j++) {
			uint val = VAL(j);
			if (val > 0 && (min2 < 0 || val < VAL(min2))) {
				if (min1 < 0 || val < VAL(min1)) {
					min2 = min1;
					min1 = j;
				} else {
					min2 = j;
				}
			}
		}
		/* printf("mins: %c %c %u %u\n", min1, min2, VAL(min1), VAL(min2)); */
		if (min2 == -1)
			break;
		htree *new = malloc(sizeof (htree));
		new->node = VAL(min1) + VAL(min2);
		if (min1 < 256) {
			new->l = calloc(1, sizeof (htree));
			new->l->node = min1;
			counts[min1] = 0;
		} else {
			new->l = treelist[min1 - 256];
			treelist[min1 - 256] = NULL;
		}
		if (min2 < 256) {
			new->r = calloc(1, sizeof (htree));
			new->r->node = min2;
			counts[min2] = 0;
		} else {
			new->r = treelist[min2 - 256];
			treelist[min2 - 256] = NULL;
		}
		treelist[treecount++] = new;
	}
	/* for (int i = 0; i < treecount; i++) { */
	/* 	if (treelist[i]) { */
	/* 		htree_print(treelist[i]); */
	/* 		putchar(10); */
	/* 		fflush(stdout); */
	/* 	} */
	/* } */
	return treecount > 0 ? treelist[treecount - 1] : NULL;
	/* filled = 0; */
	/* for (uint i = 0; i < 256; i++) { */
	/* 	if (counts[i] > 0) { */
	/* 		printf("`%c`: %2u\t", i, counts[i]); */
	/* 		filled++; */
	/* 		if (filled%5 == 0) putchar('\n'); */
	/* 	} */
	/* } */
	/* putchar(10); */
}

void htree_free(htree *h)
{
	if (h == NULL) return;
	if (h->l == NULL && h->r == NULL) {
		free(h);
		h = NULL;
		return;
	}
	htree_free(h->l);
	htree_free(h->r);
}

void DrawHuffmanTree(htree *h, Rectangle s, Vector2 prev)
{
	if (h == NULL) return;
	/* DrawRectangleLinesEx(s, 1, Fade(WHITE, 0.1)); */
	uint depth = htree_depth(h);
	uint l = h->l ? htree_count(h->l) : 1;
	uint r = h->r ? htree_count(h->r) : 1;
	Vector2 cur = {s.x+l*s.width/(l+r), s.y+R};
	char buf[4];
	if (h->l || h->r) {
		snprintf(buf, 4, "%u", h->node);
		DrawText(buf, cur.x, cur.y, 8, GREEN);
	} else {
		snprintf(buf, 4, "%c", h->node);
		if (buf[0] == ' ') strcpy(buf, "_");
		DrawText(buf, cur.x, cur.y, 20, WHITE);
	}
	if (prev.x>0) {
		DrawLineV(cur, prev, BLUE);
	}
	DrawHuffmanTree(h->l, (Rectangle) {s.x, s.y+s.height/3, cur.x-s.x, 2*s.height/3}, cur);
	DrawHuffmanTree(h->r, (Rectangle) {cur.x, s.y+s.height/3, s.x+s.width-cur.x, 2*s.height/3}, cur);
	DrawCircleV(cur, R, YELLOW);
}
