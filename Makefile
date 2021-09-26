
all: setup sbs pang

pang: pang.c
	gcc -O2 -o pang pang.c

setup: setup.c
	gcc -O2 -o setup setup.c

sbs: sbs.c
	gcc -O2 -o sbs sbs.c

