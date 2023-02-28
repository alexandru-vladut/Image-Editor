#pragma once

#include "struct.h"

#define MAXLINE 500

void free_matrix(double **a, int nl);

double **alloc_matrix(int nc, int nl);

void free_matrix_rgb(pixels **a, int nl);

pixels **alloc_matrix_rgb(int nc, int nl);

void skip_comments(FILE *in);

void my_load(char **parameters, image *imag, int *loaded, int *loaded_rgb);
