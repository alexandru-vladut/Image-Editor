#pragma once

#include "struct.h"

double **redimension_matrix(double **a, int start_x, int start_y,
							int end_x, int end_y, int nl_a);

void crop_noncolor(image *imag);

void crop_rgb(image *imag);

void crop_any(image *imag, int loaded, int loaded_rgb, int *cropped);
