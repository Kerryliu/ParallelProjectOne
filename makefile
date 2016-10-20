CC=g++ -std=c++11
CFLAGS=-c -Wall

all:qss qsp bss bsp gaussian

qss:QuickSortSequential.cpp
	$(CC) QuickSortSequential.cpp -o QuickSortSequential.o
	./QuickSortSequential.o

qsp:QuickSortParallel.cpp
	$(CC) -pthread QuickSortParallel.cpp -o QuickSortParallel.o
	./QuickSortParallel.o

bss:BitonicSortSequential.cpp
	$(CC) BitonicSortSequential.cpp -o BitonicSortSequential.o
	./BitonicSortSequential.o

bsp:BitonicSortParallel.cpp
	$(CC) -pthread BitonicSortParallel.cpp -o BitonicSortParallel.o
	./BitonicSortParallel.o

gaussian:Gaussian.cpp
	$(CC) -pthread Gaussian.cpp -o Gaussian.o
	./Gaussian.o

test:Test.cpp
	$(CC) -pthead Test.cpp -o Test.o
	./Test.o

clean:
	find . -type f -name '*.txt' -delete
	find . -type f -name '*.o' -delete
