all: compile link clean

compile: main.cpp menu.cpp game.cpp
	g++ -pedantic-errors -std=c++11 -c main.cpp menu.cpp game.cpp

link: main.o menu.o
	g++ main.o menu.o game.o -o game -lncurses

clean:
	rm *.o
