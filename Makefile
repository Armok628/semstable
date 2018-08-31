CC=clang
CFLAGS=-g -pedantic

test: test.c src/*
	$(CC) test.c src/*.c $(CFLAGS)

words: words.c src/*
	$(CC) words.c src/*.c $(CFLAGS)
