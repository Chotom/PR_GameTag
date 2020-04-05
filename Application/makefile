all: main.o Game.o GameObject.o TextureLoader.o
	g++ Debug/main.o Debug/Game.o Debug/GameObject.o Debug/TextureLoader.o -o Debug/main.out -lSDL2 -lSDL2main -lSDL2_image
main.o:
	g++ -c Client/main.cpp -o Debug/main.o
Game.o:
	g++ -c Client/Game.cpp -o Debug/Game.o
GameObject.o:
	g++ -c Client/GameObject.cpp -o Debug/GameObject.o
TextureLoader.o:
	g++ -c Client/TextureLoader.cpp -o Debug/TextureLoader.o

clean:
	rm -f Debug/*.out Debug/*.o
run: clean all
	Debug/main.out
install:
	sudo apt-get install libsdl2-dev
	sudo apt-get install libsdl2-image-dev
