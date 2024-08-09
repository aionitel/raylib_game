CFLAGS = -L lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a

dev:
	rm -rf game
	echo "Compiling game..."
	clang ${CFLAGS} src/main.c -o game
	./game
	rm -rf game

clean:
	rm -rf game
