# Make file for Socket Sample Program

OBJS = main.o tinyxml2.o
COBJS = client.o
CC = g++
DEPS = tinyxml2.h


all: main client

main: $(OBJS)
	$(CC) -o main $(OBJS)

main.o:	main.cpp $(DEPS)
	$(CC) -c main.cpp

client: client.o
	$(CC) -o client $(COBJS)

client.o: client.cpp
	$(CC) -c client.cpp

tinyxml2.o: tinyxml2.cpp tinyxml2.h
	$(CC) -c tinyxml2.cpp

# -----
# clean by removing object files

clean:
	rm $(OBJS) client.o