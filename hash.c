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
	if (!*def)
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
int main(int argc,char **argv)
{
	table_t *table=new_table(1);
	char *input=calloc(100,1),*sym=malloc(100);
	int *num=malloc(sizeof(int));
	for (;;) {
		printf("Command: ");
		fgets(input,99,stdin);
		if (!strcmp(input,"quit\n")||(*input=='\n'))
			break;
		else if (sscanf(input,"add %s %i",sym,num)==2) {
			add_entry(table,sym,num);
			printf("Adding %s as %i\n\n",sym,*num);
			num=malloc(sizeof(int));
		} else if (sscanf(input,"set %s %i",sym,num)==2) {
			set_entry(table,sym,num);
			printf("Setting %s to %i\n\n",sym,*num);
			num=malloc(sizeof(int));
		} else if (sscanf(input,"get %s",sym)==1) {
			int *ptr=(int *)get_entry(table,sym);
			if (ptr)
				printf("%i\n\n",*ptr);
			else
				printf("%s is not defined\n\n",sym);
		} else if (sscanf(input,"key %s",sym)==1) {
			printf("%s -> %lu\n\n",sym,hash_key(sym));
		} else if (!strcmp(input,"size\n")) {
			printf("%i possible entries\n\n",table->size);
		} else if (!strcmp(input,"realloc\n")) {
			table->size+=table->size+1;
			table->mem=realloc(table->mem,table->size*sizeof(bucket_t *));
		} else
			printf("Unrecognized command or format\n\n");
	}
	free(input);
	free(sym);
	free(num);
	free_table(table);
	return 0;
}
