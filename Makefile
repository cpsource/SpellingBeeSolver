
all: setup sbs worst letter tri

setup: setup.c
	gcc -O2 -o setup setup.c
	./setup

sbs: sbs.c
	gcc -O2 -o sbs sbs.c

worst: worst.c worst.h tri_list.h
	gcc -O2 -o worst worst.c

letter: letter.c
	gcc -O2 -o letter letter.c

tri: tri.c
	gcc -O2 -o tri tri.c
