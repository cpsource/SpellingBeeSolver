
all: setup letter worst tri sbs

setup: setup.c
	gcc -O2 -o setup setup.c

sbs: sbs.c
	gcc -O2 -o sbs sbs.c

worst: worst.c worst.h tri_list.h
	gcc -O2 -o worst worst.c

letter: letter.c
	gcc -O2 -o letter letter.c
	./letter

tri: tri.c
	gcc -O2 -o tri tri.c
