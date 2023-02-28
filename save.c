#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "struct.h"
#include "save.h"

void save_any(char **parameters, image imag, int loaded,
			  int loaded_rgb, int *saved)
{
	FILE *out = fopen(parameters[0], "w");
	if (!out) {
		fprintf(stderr, "Failed to open\n");
		return;
	}

	if (!strcmp(parameters[1], "ascii")) {
		if (!strcmp(imag.magic_number, "P4"))
			fprintf(out, "P1\n");
		else if (!strcmp(imag.magic_number, "P5"))
			fprintf(out, "P2\n");
		else if (!strcmp(imag.magic_number, "P6"))
			fprintf(out, "P3\n");
		else
			fprintf(out, "%s\n", imag.magic_number);

		fprintf(out, "%d %d\n", imag.width, imag.height);

		if (!strcmp(imag.magic_number, "P2") ||
			!strcmp(imag.magic_number, "P3") ||
			!strcmp(imag.magic_number, "P5") ||
			!strcmp(imag.magic_number, "P6")) {
			fprintf(out, "%d\n", imag.maxval);
		}

		if (loaded)
			for (int i = 0; i < imag.height; i++) {
				for (int j = 0; j < imag.width; j++) {
					unsigned char aux;
					aux = (unsigned char)round(imag.matrix[i][j]);
					fprintf(out, "%u ", aux);
				}
				fprintf(out, "\n");
			}
		if (loaded_rgb)
			for (int i = 0; i < imag.height; i++) {
				for (int j = 0; j < imag.width; j++) {
					unsigned char aux;
					aux = (unsigned char)round(imag.matrix_rgb[i][j].red);
					fprintf(out, "%u ", aux);
					aux = (unsigned char)round(imag.matrix_rgb[i][j].green);
					fprintf(out, "%u ", aux);
					aux = (unsigned char)round(imag.matrix_rgb[i][j].blue);
					fprintf(out, "%u ", aux);
				}
				fprintf(out, "\n");
			}

		fclose(out);
	} else {
		if (!strcmp(imag.magic_number, "P1"))
			fprintf(out, "P4\n");
		else if (!strcmp(imag.magic_number, "P2"))
			fprintf(out, "P5\n");
		else if (!strcmp(imag.magic_number, "P3"))
			fprintf(out, "P6\n");
		else
			fprintf(out, "%s\n", imag.magic_number);

		fprintf(out, "%d %d\n", imag.width, imag.height);

		if (!strcmp(imag.magic_number, "P2") ||
			!strcmp(imag.magic_number, "P3") ||
			!strcmp(imag.magic_number, "P5") ||
			!strcmp(imag.magic_number, "P6")) {
			fprintf(out, "%d\n", imag.maxval);
		}

		int pos = ftell(out);
		fclose(out);

		FILE *out_bin = fopen(parameters[0], "ab");
		fseek(out_bin, pos, SEEK_SET);

		if (loaded)
			for (int i = 0; i < imag.height; i++) {
				for (int j = 0; j < imag.width; j++) {
					unsigned char aux = (unsigned char)round(imag.matrix[i][j]);
					fwrite(&aux, sizeof(unsigned char), 1, out_bin);
				}
			}
		if (loaded_rgb)
			for (int i = 0; i < imag.height; i++) {
				for (int j = 0; j < imag.width; j++) {
					unsigned char aux_red;
					aux_red = (unsigned char)round(imag.matrix_rgb[i][j].red);
					unsigned char aux_grn;
					aux_grn = (unsigned char)round(imag.matrix_rgb[i][j].green);
					unsigned char aux_blue;
					aux_blue = (unsigned char)round(imag.matrix_rgb[i][j].blue);

					fwrite(&aux_red, sizeof(unsigned char), 1, out_bin);
					fwrite(&aux_grn, sizeof(unsigned char), 1, out_bin);
					fwrite(&aux_blue, sizeof(unsigned char), 1, out_bin);
				}
			}
		fclose(out_bin);
	}

	(*saved) = 1;
}
