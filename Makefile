CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

build:
		$(CC) $(CFLAGS) image_editor.c crop.c load.c rotate.c save.c select.c apply.c -o image_editor -lm

clean:
		rm image_editor

pack:
		zip -FSr 312CA_AlexandruNicolaeVladut_Tema3.zip README Makefile *.c *.h

.PHONY: clean pack