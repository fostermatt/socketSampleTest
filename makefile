# Make file for Socket Sample Program

OBJS = main.o tinyxml2.o
CC = g++
DEPS = tinyxml2.h


all: main

main: $(OBJS)
	$(CC) -o main $(OBJS)

main.o:	main.cpp $(DEPS)
	$(CC) -c main.cpp

tinyxml2.o: tinyxml2.cpp
	$(CC) -c tinyxml2.cpp

# -----
# clean by removing object files

clean:
	rm $(OBJS) $(COBJS)