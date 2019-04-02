CC=g++

all: test

test: test.o matrix.o
	$(CC) -o test test.o matrix.o

test.o: test.cpp matrix.h
	$(CC) -c test.cpp

matrix.o: matrix.cpp matrix.h
	$(CC) -c matrix.cpp

clean:
	rm -rf *.o test
