aes: aes.o
	g++ -std=c++11 -o aes aes.o

aes.o: aes.cpp
	g++ -std=c++11 -c aes.cpp -o aes.o

all: aes

clean:
	$(RM) aes *.o
