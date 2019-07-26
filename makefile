# Make file for Socket Sample Program

OBJS = main.o mySocket.o
COBJS = client.o mySocket.o
CC = g++
DEPS = mySocket.h


all: main client

main: $(OBJS)
	$(CC) -o main $(OBJS)

main.o:	main.cpp $(DEPS)
	$(CC) -c main.cpp

client: client.o
	$(CC) -o client $(COBJS)

client.o: client.cpp
	$(CC) -c client.cpp

mySocket.o: mySocket.cpp $(DEPS)
	$(CC) -c mySocket.cpp

# -----
# clean by removing object files

clean:
	rm $(OBJS) client.o