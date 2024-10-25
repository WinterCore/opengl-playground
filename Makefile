OS := $(shell uname)
CFLAGS = -std=c11 -Wall -Wextra
LDFLAGS = -Iinclude

ifeq ($(OS),Darwin)
	CFLAGS += `pkg-config --cflags glfw3`
	LDFLAGS += `pkg-config --static --libs glfw3`
else
	CFLAGS += `pkg-config --cflags glfw3 gl`
	LDFLAGS += `pkg-config --static --libs glfw3 gl`
endif

.PHONY: clean debug all

debug: CFLAGS += -ggdb -DDEBUG
debug: Out

release: CFLAGS += -DNDEBUG

SRC_C=$(shell find . -name "*.c")
SRC_ALL=$(shell find . -name "*.c" -o -name '*.h')

Out: $(SRC_ALL)
	cc $(CFLAGS) -o Out $(SRC_C) $(LDFLAGS)

all: Out

clean:
	rm -rf Out ./src/*.o ./src/**/*.o ./Out.dSYM
