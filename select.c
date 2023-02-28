#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "struct.h"
#include "select.h"

void selecting(char **parameters, image *imag, int *selected)
{
	int x1, y1, x2, y2, aux;

	x1 = atoi(parameters[0]);
	y1 = atoi(parameters[1]);
	x2 = atoi(parameters[2]);
	y2 = atoi(parameters[3]);

	if ((x1 < 0 || x1 > imag->width) ||
		(x2 < 0 || x2 > imag->width) ||
		(y1 < 0 || y1 > imag->height) ||
		(y2 < 0 || y2 > imag->height) ||
		(x1 == x2 && x2 == y1 && y1 == y2) ||
		(x1 == x2 || y1 == y2)) {
		printf("Invalid set of coordinates\n");
		return;
	}

	if (x1 > x2) {
		aux = x1;
		x1 = x2;
		x2 = aux;
	}

	if (y1 > y2) {
		aux = y1;
		y1 = y2;
		y2 = aux;
	}

	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);

	imag->start.x = x1;
	imag->start.y = y1;
	imag->end.x = x2;
	imag->end.y = y2;

	(*selected) = 1;
}

void selecting_all(image *imag, int *selected_all)
{
	imag->start.x = 0;
	imag->start.y = 0;
	imag->end.x = imag->width;
	imag->end.y = imag->height;

	(*selected_all) = 1;
}
