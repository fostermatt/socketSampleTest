# Make file for Socket Sample Program

OBJS = main.o tinyxml2.o server.o
CC = g++
DEPS = tinyxml2.h server.h


all: main

main: $(OBJS)
	$(CC) -o main $(OBJS)

main.o:	main.cpp $(DEPS)
	$(CC) -c main.cpp

tinyxml2.o: tinyxml2.cpp
	$(CC) -c tinyxml2.cpp

server.o: server.cpp
	$(CC) -c server.cpp

# -----
# clean by removing object files

clean:
	rm $(OBJS) $(COBJS)