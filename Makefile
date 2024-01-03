all: bin/9

bin/9: bin src/*.c include/*.h
	gcc -Wall -Wextra -o bin/9 -Iinclude src/*.c

bin:
	mkdir -p bin
