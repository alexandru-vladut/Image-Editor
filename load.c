#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "struct.h"
#include "load.h"

// functie de eliberare a unei matrice
void free_matrix(double **a, int nl)
{
	for (int i = 0; i < nl; i++)
		free(a[i]);

	free(a);
}

// functie de alocare a unei matrice
double **alloc_matrix(int nc, int nl)
{
	double **a;

	a = malloc(nl * sizeof(double *));
	if (!a)
		return NULL;

	for (int i = 0; i < nl; i++) {
		a[i] = malloc(nc * sizeof(double));
		if (!a[i]) {
			free_matrix(a, i);
			return NULL;
		}
	}

	return a;
}

// functie de eliberare a unei matrice cu elemente de tip RGB
void free_matrix_rgb(pixels **a, int nl)
{
	for (int i = 0; i < nl; i++)
		free(a[i]);

	free(a);
}

// functie de alocare a unei matrice cu elemente de tip RGB
pixels **alloc_matrix_rgb(int nc, int nl)
{
	pixels **a;

	a = malloc(nl * sizeof(pixels *));
	if (!a)
		return NULL;

	for (int i = 0; i < nl; i++) {
		a[i] = malloc(nc * sizeof(pixels));
		if (!a[i]) {
			free_matrix_rgb(a, i);
			return NULL;
		}
	}

	return a;
}

// functie pentru ignorarea comentariilor din imagine
void skip_comments(FILE *in)
{
	char c;

	fscanf(in, "%c", &c);

	while (c == '#') {
		while (c != '\n')
			fscanf(in, "%c", &c);
		fscanf(in, "%c", &c);
	}
	fseek(in, -1, SEEK_CUR);
}

// functia de LOAD
void my_load(char **parameters, image *imag, int *loaded, int *loaded_rgb)
{
	// daca exista o matrice deja incarcata, eliberam memoria
	if ((*loaded) == 1) {
		free_matrix(imag->matrix, imag->height);
		(*loaded) = 0;
	}
	if ((*loaded_rgb) == 1) {
		free_matrix_rgb(imag->matrix_rgb, imag->height);
		(*loaded_rgb) = 0;
	}
	// deschiderea fisierului pentru read ASCII + progr. defensiva
	FILE *in = fopen(parameters[0], "r");
	if (!in)
		return;

	// ignoram eventualele comentarii aflate inainte de magic_number
	skip_comments(in);

	fgets(imag->magic_number, 5, in);
	imag->magic_number[2] = '\0';

	// ignoram eventualele comentarii aflate inainte de width si height
	skip_comments(in);

	fscanf(in, "%d%d", &imag->width, &imag->height);
	// ignoram '\n' ramas si eventualele comentarii aflate
	// inainte de elementele matricei / maxval
	fseek(in, 1, SEEK_CUR);
	skip_comments(in);

	if (!strcmp(imag->magic_number, "P1")) {
		// alocarea matricei + progr. defensiva
		imag->matrix = alloc_matrix(imag->width, imag->height);
		if (!imag->matrix) {
			fprintf(stderr, "Failed allocation\n");
			fclose(in);
			exit(-1);
		}

		// popularea matricei
		for (int i = 0; i < imag->height; i++)
			for (int j = 0; j < imag->width; j++)
				fscanf(in, "%lf", &imag->matrix[i][j]);

		fclose(in);
		(*loaded) = 1;

	} else if (!strcmp(imag->magic_number, "P2")) {
		fscanf(in, "%d", &imag->maxval);
		fseek(in, 1, SEEK_CUR);

		skip_comments(in);

		// alocarea matricei + progr. defensiva
		imag->matrix = alloc_matrix(imag->width, imag->height);
		if (!imag->matrix) {
			fprintf(stderr, "Failed allocation\n");
			fclose(in);
			exit(-1);
		}

		// popularea matricei
		for (int i = 0; i < imag->height; i++)
			for (int j = 0; j < imag->width; j++)
				fscanf(in, "%lf", &imag->matrix[i][j]);

		fclose(in);
		(*loaded) = 1;

	} else if (!strcmp(imag->magic_number, "P4")) {
		int pos = ftell(in);
		fclose(in);

		// deschiderea fisierului pentru read BINAR + progr. defensiva
		FILE *in_bin = fopen(parameters[0], "rb");
		if (!in_bin)
			return;
		fseek(in_bin, pos, SEEK_SET);

		// alocarea matricei + progr. defensiva
		imag->matrix = alloc_matrix(imag->width, imag->height);
		if (!imag->matrix) {
			fprintf(stderr, "Failed allocation\n");
			fclose(in);
			exit(-1);
		}

		// popularea matricei
		for (int i = 0; i < imag->height; i++)
			for (int j = 0; j < imag->width; j++) {
				unsigned char aux;
				fread(&aux, sizeof(unsigned char), 1, in_bin);
				imag->matrix[i][j] = aux;
			}

		fclose(in_bin);
		(*loaded) = 1;

	} else if (!strcmp(imag->magic_number, "P5")) {
		fscanf(in, "%d", &imag->maxval);
		fseek(in, 1, SEEK_CUR);

		skip_comments(in);

		int pos = ftell(in);
		fclose(in);

		// deschiderea fisierului pentru read BINAR + progr. defensiva
		FILE *in_bin = fopen(parameters[0], "rb");
		if (!in_bin)
			return;
		fseek(in_bin, pos, SEEK_SET);

		// alocarea matricei + progr. defensiva
		imag->matrix = alloc_matrix(imag->width, imag->height);
		if (!imag->matrix) {
			fprintf(stderr, "Failed allocation\n");
			fclose(in);
			exit(-1);
		}

		// popularea matricei
		for (int i = 0; i < imag->height; i++)
			for (int j = 0; j < imag->width; j++) {
				unsigned char aux;
				fread(&aux, sizeof(unsigned char), 1, in_bin);
				imag->matrix[i][j] = aux;
			}

		fclose(in_bin);
		(*loaded) = 1;

	} else if (!strcmp(imag->magic_number, "P3")) {
		fscanf(in, "%d", &imag->maxval);
		fseek(in, 1, SEEK_CUR);

		skip_comments(in);

		// alocarea matricei RGB + progr. defensiva
		imag->matrix_rgb = alloc_matrix_rgb(imag->width, imag->height);
		if (!imag->matrix_rgb) {
			fprintf(stderr, "Failed allocation\n");
			fclose(in);
			exit(-1);
		}

		// popularea matricei RGB
		for (int i = 0; i < imag->height; i++)
			for (int j = 0; j < imag->width; j++) {
				fscanf(in, "%lf", &imag->matrix_rgb[i][j].red);
				fscanf(in, "%lf", &imag->matrix_rgb[i][j].green);
				fscanf(in, "%lf", &imag->matrix_rgb[i][j].blue);
			}

		fclose(in);
		(*loaded_rgb) = 1;

	} else if (strcmp(imag->magic_number, "P6") == 0) {
		fscanf(in, "%d", &imag->maxval);
		fseek(in, 1, SEEK_CUR);

		skip_comments(in);

		int pos = ftell(in);
		fclose(in);

		// deschiderea fisierului pentru read BINAR + progr. defensiva
		FILE *in_bin = fopen(parameters[0], "rb");
		if (!in_bin)
			return;
		fseek(in_bin, pos, SEEK_SET);

		// alocarea matricei RGB + progr. defensiva
		imag->matrix_rgb = alloc_matrix_rgb(imag->width, imag->height);
		if (!imag->matrix_rgb) {
			fprintf(stderr, "Failed allocation\n");
			exit(-1);
		}
		// popularea matricei
		for (int i = 0; i < imag->height; i++)
			for (int j = 0; j < imag->width; j++) {
				unsigned char aux_red, aux_green, aux_blue;

				fread(&aux_red, sizeof(unsigned char), 1, in_bin);
				imag->matrix_rgb[i][j].red = aux_red;

				fread(&aux_green, sizeof(unsigned char), 1, in_bin);
				imag->matrix_rgb[i][j].green = aux_green;

				fread(&aux_blue, sizeof(unsigned char), 1, in_bin);
				imag->matrix_rgb[i][j].blue = aux_blue;
			}
		fclose(in_bin);
		(*loaded_rgb) = 1;
	}
}
