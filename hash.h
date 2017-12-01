#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct bucket_s {
	unsigned long key;
	void *val;
	//struct bucket_s *cdr;
} bucket_t;
typedef struct {
	int size; // # of possible entries
	int initsize;
	bucket_t **mem;
} table_t;
unsigned long hash_key(char *str) // lazy
{
	unsigned long key=0;
	for (char *c=str;*c;c++) {
		key+=*c;
		key<<=8;
		key/=*c;
		key+=*c;
	}
	//fprintf(stderr,"-- Key: %s -> %lu --\n",str,key);
	return key;
}
bucket_t* new_bucket(unsigned long key,void *val)
{
	bucket_t *b=malloc(sizeof(bucket_t));
	b->key=key;
	b->val=val;
	return b;
}
void free_bucket(bucket_t *b)
{
	free(b->val);
	free(b);
}
table_t *new_table(int size) {
	table_t *table=malloc(sizeof(table_t));
	table->mem=calloc(size,sizeof(bucket_t *));
	table->size=size;
	table->initsize=size;
	return table;
}
void free_table(table_t *table)
{
	for (int i=0;i<table->size;i++)
		if (table->mem[i])
			free_bucket(table->mem[i]);
	free(table->mem);
	free(table);
}
bucket_t *add_bucket(table_t *table,unsigned long key,bucket_t *entry)
{
	bucket_t **def=&table->mem[key%table->size];
	if (*def) { // Hash collision. Double the memory bank and add one
		fprintf(stderr,"-- Collision: Reallocating memory --\n");
		table->size<<=1;
		table->size++;
		table->mem=realloc(table->mem,table->size*sizeof(bucket_t *));
		return add_bucket(table,key,entry);
	} else
		*def=entry;
	return *def;
}
bucket_t *add_entry(table_t *table,char *str,void *entry)
{
	unsigned long key=hash_key(str);
	bucket_t *bucket=new_bucket(key,entry);
	return add_bucket(table,key,bucket);
}
void *get_entry(table_t *table,char *str)
{
	unsigned long key=hash_key(str);
	int size=table->size;
	bucket_t **def=&table->mem[key%size];
	if (*def&&(*def)->key==key)
		return (*def)->val;
	fprintf(stderr,"-- Entry not found where expected --\n");
	while (size>table->initsize&&!(*def&&(*def)->key==key)) {
		fprintf(stderr,"-- Checking next possible address --\n");
		size--;
		size>>=1;
		def=&table->mem[key%size];
	}
	if (!*def||(*def)->key!=key)
		return NULL; // Not defined
	fprintf(stderr,"-- Found: Relocating --\n");
	bucket_t *b=add_bucket(table,key,*def); // Give it a newer entry
	*def=NULL; // Remove the old one
	return b->val;
}
void set_entry(table_t *table,char *str,void *entry) // add_entry() without collision handling
{
	unsigned long key=hash_key(str);
	bucket_t **def=&table->mem[key%table->size];
	if (*def)
		free_bucket(*def);
	*def=new_bucket(key,entry);
}
