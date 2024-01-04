all: bin/9

bin/9: bin src/*.c include/*.h
	gcc -Wall -Wextra -o bin/9 -Iinclude src/*.c

bin/9test: bin src/*.c include/*.h
	gcc -DDEBUG=true -DOUT_IS_ERROR=true -DSTACK_SIZE=512 -Wall -Wextra -o bin/9test -Iinclude src/*.c

test: bin/9test

install: bin/9
	cp bin/9 /bin/9

bin:
	mkdir -p bin
