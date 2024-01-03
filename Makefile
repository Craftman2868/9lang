all: bin/9

bin/9: bin src/*.c include/*.h
	gcc -Wall -Wextra -o bin/9 -Iinclude src/*.c

bin/9test: bin src/*.c include/*.h
	gcc -DDEBUG=true -DOUT_IS_ERROR=true -Wall -Wextra -o bin/9test -Iinclude src/*.c

test: bin/9test

bin:
	mkdir -p bin
