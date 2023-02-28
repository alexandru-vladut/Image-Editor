#pragma once

#define COMSIZE 40
#define MAXPAR 4
#define MAXCOM 5

typedef struct coordinates {
	int x;
	int y;
} coords;

typedef struct pixels {
	double red;
	double green;
	double blue;
} pixels;

typedef struct image {
	char magic_number[2];
	int width;
	int height;
	unsigned int maxval;
	double **matrix;
	pixels **matrix_rgb;
	coords start;
	coords end;
} image;

typedef enum commands {
	NO,
	LOAD,
	SELECT,
	SELECT_ALL,
	ROTATE,
	CROP,
	APPLY,
	SAVE,
	EXIT
} commands;
