#pragma once

#include "struct.h"
#include "crop.h"

double clamp_double(double x);

void apply_edge(image *imag, pixels **aux);

void apply_sharpen(image *imag, pixels **aux);

void apply_blur(image *imag, pixels **aux);

void apply_gaussianblur(image *imag, pixels **aux);

void apply_any(char **parameters, image *imag, int *applied);
