CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

SOURCE = $(wildcard *.c)
OBJECT = $(wildcard *.o)

all: encode decode error entropy

encode: encode.o bm.o bv.o hamming.o
	$(CC) -o encode encode.o bm.o bv.o hamming.o
decode: decode.o bm.o bv.o hamming.o
	$(CC) -o decode decode.o bm.o bv.o hamming.o
error: error.o
	$(CC) -o error error.o
entropy: entropy.o
	$(CC) -o entropy entropy.o -lm
%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECT)

scan-build: clean
	scan-build make
format:
	clang-format -i -style=file *.c *.h
