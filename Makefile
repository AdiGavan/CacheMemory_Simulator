#Gavan Adrian George - 314CA

build: tema1

tema1: tema1.o
	g++ -Wall tema1.o -o tema1

tema1.o: tema1.cc
	g++ -Wall -c tema1.cc

clean:
	rm -f *.o tema1
