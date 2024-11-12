.PHONY: all clean run
all: main

main: main.cpp work.h libwork.so Makefile
	g++ -std=c++11 -Wall -Wextra -O2 $< -L. -l:libwork.so -o $@ -lpthread

run: main
	./main ${SEED}

clean: Makefile
	-@rm main
