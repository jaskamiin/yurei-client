CC=g++
CFLAGS=-Wall -Wpedantic -std=c++14 -g
SOURCES=*.cpp
EXECUTABLE=yurei

all: yurei

yurei: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -rf *o $(EXECUTABLE)
