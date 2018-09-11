CC=clang
CFLAGS=-g -std=c89 -Wall -Wextra -Wpedantic

test: test.c src/*
	$(CC) test.c src/*.c $(CFLAGS)

words: words.c src/*
	$(CC) words.c src/*.c $(CFLAGS)
