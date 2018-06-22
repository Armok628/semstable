#ifndef HASH_H
#define HASH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct bucket_s {
	unsigned long key;
	void *val;
	struct bucket_s *cdr;
} bucket_t;
typedef struct {
	int size; // # of possible entries
	bucket_t **mem;
	void (*destructor)(void *);
} table_t;

table_t *new_table(int,void (*)(void *));
void free_table(table_t *);
bucket_t *add_entry(table_t *,char *,void *);
void *get_entry(table_t *,char *);
void set_entry(table_t *,char *,void *);
#endif
