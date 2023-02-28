#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "struct.h"
#include "crop.h"
#include "load.h"

double **redimension_matrix(double **a, int start_x, int start_y,
							int end_x, int end_y, int nl_a)
{
	double **redimension = alloc_matrix(end_x - start_x, end_y - start_y);
	if (!redimension)
		return NULL;

	for (int i = 0; i < end_y - start_y; i++) {
		for (int j = 0; j < end_x - start_x; j++)
			redimension[i][j] = a[start_y + i][start_x + j];
	}

	free_matrix(a, nl_a);

	return redimension;
}

void crop_noncolor(image *imag)
{
	imag->matrix = redimension_matrix(imag->matrix, imag->start.x,
									  imag->start.y, imag->end.x, imag->end.y,
									  imag->height);

	if (!imag->matrix) {
		fprintf(stderr, "Failed to reallocate\n");
		return;
	}

	imag->height = imag->end.y - imag->start.y;
	imag->width = imag->end.x - imag->start.x;

	imag->start.x = 0;
	imag->start.y = 0;
	imag->end.x = imag->width;
	imag->end.y = imag->height;
}

pixels **redimension_matrix_rgb(pixels **a, int start_x, int start_y,
								int end_x, int end_y, int nl_a)
{
	pixels **redimension = alloc_matrix_rgb(end_x - start_x, end_y - start_y);
	if (!redimension)
		return NULL;

	for (int i = 0; i < end_y - start_y; i++)
		for (int j = 0; j < end_x - start_x; j++)
			redimension[i][j] = a[start_y + i][start_x + j];

	free_matrix_rgb(a, nl_a);

	return redimension;
}

void crop_rgb(image *imag)
{
	imag->matrix_rgb = redimension_matrix_rgb(imag->matrix_rgb, imag->start.x,
											  imag->start.y, imag->end.x,
											  imag->end.y, imag->height);

	if (!imag->matrix_rgb) {
		fprintf(stderr, "Failed to reallocate\n");
		return;
	}

	imag->height = imag->end.y - imag->start.y;
	imag->width = imag->end.x - imag->start.x;

	imag->start.x = 0;
	imag->start.y = 0;
	imag->end.x = imag->width;
	imag->end.y = imag->height;
}

void crop_any(image *imag, int loaded, int loaded_rgb, int *cropped)
{
	if (loaded)
		crop_noncolor(imag);

	if (loaded_rgb)
		crop_rgb(imag);

	(*cropped) = 1;
}
