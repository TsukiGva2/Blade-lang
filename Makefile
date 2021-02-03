CC=g++

all:
	$(CC) blade.cpp -o blade -static-libgcc -static-libstdc++
