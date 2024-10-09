CC = clang
INCLUDE = -I./include include/raylib.h
LIBS = -L./lib -lraylib -ldl
SRC_FILES = src/main.c
FRAMEWORK = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation

build:
	${CC} ${LIBS} ${INCLUDE} ${SRC_FILES} ${FRAMEWORK}
	./a.out
	rm -rf ./a.out
