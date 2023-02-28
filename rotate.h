#pragma once

#include "struct.h"
#include "crop.h"

void rotate_noncolor(image * imag);

void rotate_rgb(image *imag);

void rotate_any(image *imag, char **parameters, int loaded,
				int loaded_rgb, int *rotated);
