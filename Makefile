all: boostPool stdPool
clean:
	rm -f busyloop.o boostPool stdPool

boostPool: busyloop.o example.cpp threadPool.hpp
	g++ example.cpp busyloop.o -o boostPool -std=c++14 -O3 -lpthread -DWITHBOOST -lboost_system -lboost_thread

stdPool: busyloop.o example.cpp ThreadPool.hpp
	g++ example.cpp busyloop.o -o stdPool -std=c++14 -O3 -lpthread

busyloop.o: busyloop.c
	g++ busyloop.c -c -o busyloop.o

