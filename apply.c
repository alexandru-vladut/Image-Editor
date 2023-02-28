#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "struct.h"
#include "crop.h"
#include "load.h"
#include "apply.h"

double clamp_double(double x)
{
	if (x < 0)
		return 0;
	if (x > 255)
		return 255;

	return x;
}

void apply_edge(image *imag, pixels **aux)
{
	for (int i = imag->start.y; i < imag->end.y; i++)
		for (int j = imag->start.x; j < imag->end.x; j++) {
			if (i == 0 || i == imag->height - 1 ||
				j == 0 || j == imag->width - 1) {
				aux[i][j] = imag->matrix_rgb[i][j];
			} else {
				aux[i][j].red = imag->matrix_rgb[i][j].red * 8 -
					(imag->matrix_rgb[i - 1][j - 1].red +
					imag->matrix_rgb[i - 1][j].red +
					imag->matrix_rgb[i - 1][j + 1].red +
					imag->matrix_rgb[i][j - 1].red +
					imag->matrix_rgb[i][j + 1].red +
					imag->matrix_rgb[i + 1][j - 1].red +
					imag->matrix_rgb[i + 1][j].red +
					imag->matrix_rgb[i + 1][j + 1].red);

				aux[i][j].green = imag->matrix_rgb[i][j].green * 8 -
					(imag->matrix_rgb[i - 1][j - 1].green +
					imag->matrix_rgb[i - 1][j].green +
					imag->matrix_rgb[i - 1][j + 1].green +
					imag->matrix_rgb[i][j - 1].green +
					imag->matrix_rgb[i][j + 1].green +
					imag->matrix_rgb[i + 1][j - 1].green +
					imag->matrix_rgb[i + 1][j].green +
					imag->matrix_rgb[i + 1][j + 1].green);

				aux[i][j].blue = imag->matrix_rgb[i][j].blue * 8 -
					(imag->matrix_rgb[i - 1][j - 1].blue +
					imag->matrix_rgb[i - 1][j].blue +
					imag->matrix_rgb[i - 1][j + 1].blue +
					imag->matrix_rgb[i][j - 1].blue +
					imag->matrix_rgb[i][j + 1].blue +
					imag->matrix_rgb[i + 1][j - 1].blue +
					imag->matrix_rgb[i + 1][j].blue +
					imag->matrix_rgb[i + 1][j + 1].blue);
			}
		}
}

void apply_sharpen(image *imag, pixels **aux)
{
	for (int i = imag->start.y; i < imag->end.y; i++)
		for (int j = imag->start.x; j < imag->end.x; j++) {
			if (i == 0 || i == imag->height - 1 ||
				j == 0 || j == imag->width - 1) {
				aux[i][j] = imag->matrix_rgb[i][j];
			} else {
				aux[i][j].red = imag->matrix_rgb[i][j].red * 5 -
					(imag->matrix_rgb[i - 1][j].red +
					imag->matrix_rgb[i][j - 1].red +
					imag->matrix_rgb[i][j + 1].red +
					imag->matrix_rgb[i + 1][j].red);

				aux[i][j].green = imag->matrix_rgb[i][j].green * 5 -
					(imag->matrix_rgb[i - 1][j].green +
					imag->matrix_rgb[i][j - 1].green +
					imag->matrix_rgb[i][j + 1].green +
					imag->matrix_rgb[i + 1][j].green);

				aux[i][j].blue = imag->matrix_rgb[i][j].blue * 5 -
					(imag->matrix_rgb[i - 1][j].blue +
					imag->matrix_rgb[i][j - 1].blue +
					imag->matrix_rgb[i][j + 1].blue +
					imag->matrix_rgb[i + 1][j].blue);
			}
		}
}

void apply_blur(image *imag, pixels **aux)
{
	for (int i = imag->start.y; i < imag->end.y; i++)
		for (int j = imag->start.x; j < imag->end.x; j++) {
			if (i == 0 || i == imag->height - 1 ||
				j == 0 || j == imag->width - 1) {
				aux[i][j] = imag->matrix_rgb[i][j];
			} else {
				aux[i][j].red = (imag->matrix_rgb[i][j].red +
					imag->matrix_rgb[i - 1][j - 1].red +
					imag->matrix_rgb[i - 1][j].red +
					imag->matrix_rgb[i - 1][j + 1].red +
					imag->matrix_rgb[i][j - 1].red +
					imag->matrix_rgb[i][j + 1].red +
					imag->matrix_rgb[i + 1][j - 1].red +
					imag->matrix_rgb[i + 1][j].red +
					imag->matrix_rgb[i + 1][j + 1].red) / 9;

				aux[i][j].green = (imag->matrix_rgb[i][j].green +
					imag->matrix_rgb[i - 1][j - 1].green +
					imag->matrix_rgb[i - 1][j].green +
					imag->matrix_rgb[i - 1][j + 1].green +
					imag->matrix_rgb[i][j - 1].green +
					imag->matrix_rgb[i][j + 1].green +
					imag->matrix_rgb[i + 1][j - 1].green +
					imag->matrix_rgb[i + 1][j].green +
					imag->matrix_rgb[i + 1][j + 1].green) / 9;

				aux[i][j].blue = (imag->matrix_rgb[i][j].blue +
					imag->matrix_rgb[i - 1][j - 1].blue +
					imag->matrix_rgb[i - 1][j].blue +
					imag->matrix_rgb[i - 1][j + 1].blue +
					imag->matrix_rgb[i][j - 1].blue +
					imag->matrix_rgb[i][j + 1].blue +
					imag->matrix_rgb[i + 1][j - 1].blue +
					imag->matrix_rgb[i + 1][j].blue +
					imag->matrix_rgb[i + 1][j + 1].blue) / 9;
			}
		}
}

void apply_gaussianblur(image *imag, pixels **aux)
{
	for (int i = imag->start.y; i < imag->end.y; i++)
		for (int j = imag->start.x; j < imag->end.x; j++) {
			if (i == 0 || i == imag->height - 1 ||
				j == 0 || j == imag->width - 1) {
				aux[i][j] = imag->matrix_rgb[i][j];
			} else {
				aux[i][j].red = (imag->matrix_rgb[i][j].red * 4 +
					imag->matrix_rgb[i - 1][j - 1].red +
					imag->matrix_rgb[i - 1][j].red * 2 +
					imag->matrix_rgb[i - 1][j + 1].red +
					imag->matrix_rgb[i][j - 1].red * 2 +
					imag->matrix_rgb[i][j + 1].red * 2 +
					imag->matrix_rgb[i + 1][j - 1].red +
					imag->matrix_rgb[i + 1][j].red * 2 +
					imag->matrix_rgb[i + 1][j + 1].red) / 16;

				aux[i][j].green = (imag->matrix_rgb[i][j].green * 4 +
					imag->matrix_rgb[i - 1][j - 1].green +
					imag->matrix_rgb[i - 1][j].green * 2 +
					imag->matrix_rgb[i - 1][j + 1].green +
					imag->matrix_rgb[i][j - 1].green * 2 +
					imag->matrix_rgb[i][j + 1].green * 2 +
					imag->matrix_rgb[i + 1][j - 1].green +
					imag->matrix_rgb[i + 1][j].green * 2 +
					imag->matrix_rgb[i + 1][j + 1].green) / 16;

				aux[i][j].blue = (imag->matrix_rgb[i][j].blue * 4 +
					imag->matrix_rgb[i - 1][j - 1].blue +
					imag->matrix_rgb[i - 1][j].blue * 2 +
					imag->matrix_rgb[i - 1][j + 1].blue +
					imag->matrix_rgb[i][j - 1].blue * 2 +
					imag->matrix_rgb[i][j + 1].blue * 2 +
					imag->matrix_rgb[i + 1][j - 1].blue +
					imag->matrix_rgb[i + 1][j].blue * 2 +
					imag->matrix_rgb[i + 1][j + 1].blue) / 16;
			}
		}
}

void apply_any(char **parameters, image *imag, int *applied)
{
	pixels **aux = alloc_matrix_rgb(imag->width, imag->height);
	if (!aux) {
		fprintf(stderr, "Failed to allocate\n");
		return;
	}

	if (!strcmp(parameters[0], "EDGE"))
		apply_edge(imag, aux);
	else if (!strcmp(parameters[0], "SHARPEN"))
		apply_sharpen(imag, aux);
	else if (!strcmp(parameters[0], "BLUR"))
		apply_blur(imag, aux);
	else if (!strcmp(parameters[0], "GAUSSIAN_BLUR"))
		apply_gaussianblur(imag, aux);
	else
		return;

	for (int i = imag->start.y; i < imag->end.y; i++)
		for (int j = imag->start.x; j < imag->end.x; j++) {
			imag->matrix_rgb[i][j].red = clamp_double(aux[i][j].red);
			imag->matrix_rgb[i][j].green = clamp_double(aux[i][j].green);
			imag->matrix_rgb[i][j].blue = clamp_double(aux[i][j].blue);
		}

	free_matrix_rgb(aux, imag->height);

	(*applied) = 1;
}
