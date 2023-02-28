#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "struct.h"
#include "crop.h"
#include "load.h"

void swap(double *x, double *y)
{
	double aux = *x;
	*x = *y;
	*y = aux;
}

void swap_rgb(pixels *x, pixels *y)
{
	pixels aux = *x;
	*x = *y;
	*y = aux;
}

void rotate_noncolor(image *imag)
{
	double **a = alloc_matrix(imag->width, imag->height);
	if (!a) {
		fprintf(stderr, "Failed to allocate\n");
		return;
	}

	for (int i = imag->start.y; i < imag->end.y; i++)
		for (int j = imag->start.x; j < imag->end.x; j++)
			a[i - imag->start.y][j - imag->start.x] = imag->matrix[i][j];

	int n = imag->end.x - imag->start.x;

	for (int i = 0; i < n / 2; i++)
		for (int j = i; j < n - i - 1; j++) {
			double aux = a[i][j];
			a[i][j] = a[n - 1 - j][i];
			a[n - 1 - j][i] = a[n - 1 - i][n - 1 - j];
			a[n - 1 - i][n - 1 - j] = a[j][n - 1 - i];
			a[j][n - 1 - i] = aux;
		}

	for (int i = imag->start.y; i < imag->end.y; i++)
		for (int j = imag->start.x; j < imag->end.x; j++)
			imag->matrix[i][j] = a[i - imag->start.y][j - imag->start.x];

	free_matrix(a, imag->height);
}

void rotate_noncolor_all(image *imag)
{
	double **a = alloc_matrix(imag->height, imag->width);
	if (!a) {
		fprintf(stderr, "Failed to allocate\n");
		exit(-1);
	}

	for (int i = 0; i < imag->width; i++)
		for (int j = 0; j < imag->height; j++)
			a[i][j] = imag->matrix[imag->height - j - 1][i];

	free_matrix(imag->matrix, imag->height);
	imag->matrix = alloc_matrix(imag->height, imag->width);

	for (int i = 0; i < imag->end.x; i++)
		for (int j = 0; j < imag->end.y; j++)
			imag->matrix[i][j] = a[i][j];

	int aux = imag->end.x;
	imag->end.x = imag->end.y;
	imag->end.y = aux;

	aux = imag->height;
	imag->height = imag->width;
	imag->width = aux;

	free_matrix(a, imag->height);
}

void rotate_rgb(image *imag)
{
	pixels **a = alloc_matrix_rgb(imag->width, imag->height);
	if (!a) {
		fprintf(stderr, "Failed to allocate\n");
		return;
	}

	for (int i = imag->start.y; i < imag->end.y; i++)
		for (int j = imag->start.x; j < imag->end.x; j++)
			a[i - imag->start.y][j - imag->start.x] = imag->matrix_rgb[i][j];

	int n = imag->end.x - imag->start.x;

	for (int i = 0; i < n / 2; i++)
		for (int j = i; j < n - i - 1; j++) {
			pixels aux = a[i][j];
			a[i][j] = a[n - 1 - j][i];
			a[n - 1 - j][i] = a[n - 1 - i][n - 1 - j];
			a[n - 1 - i][n - 1 - j] = a[j][n - 1 - i];
			a[j][n - 1 - i] = aux;
		}

	for (int i = imag->start.y; i < imag->end.y; i++)
		for (int j = imag->start.x; j < imag->end.x; j++)
			imag->matrix_rgb[i][j] = a[i - imag->start.y][j - imag->start.x];

	free_matrix_rgb(a, imag->height);
}

void rotate_rgb_all(image *imag)
{
	pixels **a = alloc_matrix_rgb(imag->height, imag->width);
	if (!a) {
		fprintf(stderr, "Failed to allocate\n");
		exit(-1);
	}

	for (int i = 0; i < imag->width; i++)
		for (int j = 0; j < imag->height; j++)
			a[i][j] = imag->matrix_rgb[imag->height - j - 1][i];

	free_matrix_rgb(imag->matrix_rgb, imag->height);
	imag->matrix_rgb = alloc_matrix_rgb(imag->height, imag->width);

	for (int i = 0; i < imag->end.x; i++)
		for (int j = 0; j < imag->end.y; j++)
			imag->matrix_rgb[i][j] = a[i][j];

	int aux = imag->end.x;
	imag->end.x = imag->end.y;
	imag->end.y = aux;

	aux = imag->height;
	imag->height = imag->width;
	imag->width = aux;

	free_matrix_rgb(a, imag->height);
}

void rotate_any(image *imag, char **parameters, int loaded,
				int loaded_rgb, int *rotated)
{
	if (imag->end.x - imag->start.x != imag->end.y - imag->start.y &&
		(imag->start.x != 0 && imag->start.y != 0 &&
		imag->end.x != imag->width && imag->end.y != imag->height)) {
		printf("The selection must be square\n");
		return;
	}

	int angle = atoi(parameters[0]);

	if (angle % 90 != 0 || angle > 360 || angle < -360) {
		printf("Unsupported rotation angle\n");
		return;
	}

	if (angle < 0)
		angle = 360 + angle;

	for (int nr = 0; nr < angle / 90; nr++) {
		if (loaded) {
			if (imag->start.x == 0 && imag->start.y == 0 &&
				imag->end.x == imag->width && imag->end.y == imag->height) {
				rotate_noncolor_all(imag);
			} else {
				rotate_noncolor(imag);
			}
		} else if (loaded_rgb) {
			if (imag->start.x == 0 && imag->start.y == 0 &&
				imag->end.x == imag->width && imag->end.y == imag->height) {
				rotate_rgb_all(imag);
			} else {
				rotate_rgb(imag);
			}
		}
	}

	(*rotated) = 1;
}
