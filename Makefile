CC=clang
CFLAGS=-s -Os -pedantic

hashtest: test.c src/*
	$(CC) test.c src/*.c $(CFLAGS)
