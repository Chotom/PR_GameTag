all: main.o Game.o
	g++ Debug/main.o Debug/Game.o -o Debug/main.out `pkg-config --cflags --libs sdl2`
main.o:
	g++ -c Client/main.cpp -o Debug/main.o
Game.o:
	g++ -c Client/Game.cpp -o Debug/Game.o

clean:
	rm -f Debug/*.out Debug/*.o
run: clean all
	Debug/main.out
