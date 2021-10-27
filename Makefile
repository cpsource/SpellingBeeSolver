
GIT_VERSION := "$(shell git describe --abbrev=8 --dirty --always --tags)"
BUILD_DATE  := "$(shell date)"

all: setup sbs pang

pang: pang.c
	gcc -O2 -o pang pang.c

setup: config.h setup.c
	gcc -O2 -o setup setup.c

sbs: config.h version.h sbs.c
	gcc -DGIT_VERSION=\"$(GIT_VERSION)\" -DBUILD_DATE=\"$(BUILD_DATE)\" -O2 -o sbs sbs.c

