
GIT_VERSION := "$(shell git describe --abbrev=8 --dirty --always --tags)"

all: setup sbs pang

pang: pang.c
	gcc -O2 -o pang pang.c

setup: setup.c
	gcc -O2 -o setup setup.c

sbs: version.h sbs.c
	gcc -DGIT_VERSION=\"$(GIT_VERSION)\" -O2 -o sbs sbs.c

