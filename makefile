CC = gcc
LD = $(CC) -fsanitize=undefined -fsanitize=address
CFLAGS = -std=c11\
		 -g\
		 -Wall\
		 -pedantic\
		 -fsanitize=undefined\
		 -fsanitize=address

.PHONY: main
main:
	$(LD) $(CFLAGS) -o main main.c -lmpfr -lgmp -lm

.PHONY: mpfr_version
mpfr_version:
	$(LD) $(CFLAGS) -o mpfr_version mpfr_version.c -lmpfr -lgmp