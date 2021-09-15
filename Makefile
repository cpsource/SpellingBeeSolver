
all: setup sbs

setup: setup.c
	gcc -O2 -o setup setup.c
	./setup

sbs: sbs.c
	gcc -O2 -o sbs sbs.c
