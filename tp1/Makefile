all:
	g++ -Wall -std=c++11 -c friend.cpp
	g++ -Wall -std=c++11 -c monitor.cpp
	g++ -Wall -std=c++11 main.cpp friend.o monitor.o -lpthread -o tp1

clean:
	rm monitor.o friend.o tp1
