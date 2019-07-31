# Make file for Socket Sample Program
ifeq ($(UNAME_S),Linux)
	CC = g++ -std=c++11
else
	CC = g++
endif

OBJS = main.o tinyxml2.o server.o
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