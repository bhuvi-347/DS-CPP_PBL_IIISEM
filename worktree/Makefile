CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c11
CXXFLAGS = -Wall -Wextra -std=c++11

lostfound: ds.o oops.o
	$(CXX) -o lostfound ds.o oops.o

ds.o: ds.c item.h
	$(CC) $(CFLAGS) -c ds.c

oops.o: oops.cpp item.h
	$(CXX) $(CXXFLAGS) -c oops.cpp

clean:
	rm -f ds.o oops.o lostfound lostfound.exe
