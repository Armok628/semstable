#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	unsigned long key;
	void *num;
} bucket_t; // To-do: Work this in
typedef struct {
	int size; // # of possible entries
	int initsize;
	int **mem;
	//bucket_t **mem;
} hash_t;
unsigned long hash_key(char *str) // lazy
{
	unsigned long key=0;
	for (char *c=str;*c;c++) {
		key<<=8;
		key+=*c*13;
		key<<=8;
		key/=*c*17;
		key>>=2;
		key*=*c**c*23;
		key<<=3;
		key-=*c+31;
	}
	//fprintf(stderr,"-- Key: %s -> %lu --\n",str,key);
	return key;
}
hash_t *new_hash(int size) {
	hash_t *table=malloc(sizeof(hash_t));
	table->mem=calloc(size,sizeof(int *));
	table->size=size;
	table->initsize=size;
	return table;
}
void set_entry(hash_t *table,char *str,int *entry)
{
	int **def=&table->mem[hash_key(str)%table->size];
	if (*def)
		free(*def);
	*def=entry;
}
void add_entry(hash_t *table,char *str,int *entry) // set_entry but handles collisions
{
	int **def=&table->mem[hash_key(str)%table->size];
	if (*def) { // Hash collision. Double the memory bank and add one
		fprintf(stderr,"-- Collision: Reallocating memory --\n");
		table->size<<=1;
		table->mem=realloc(table->mem,table->size*sizeof(int *));
		add_entry(table,str,entry);
	} else
		*def=entry;
}
int *get_entry(hash_t *table,char *str)
{
	unsigned long key=hash_key(str);
	int size=table->size;
	int **def=&table->mem[key%size];
	if (*def)
		return *def;
	fprintf(stderr,"-- Entry not found where expected --\n");
	while (!*def&&size>table->initsize) {
		fprintf(stderr,"-- Checking next possible address --\n");
		size>>=1;
		def=&table->mem[key%size];
	}
	if (!*def)
		return NULL; // Not defined
	fprintf(stderr,"-- Found: Relocating 0x%x --\n",*def);
	add_entry(table,str,*def); // Give it a newer entry
	*def=NULL; // Remove the old one
	return get_entry(table,str); // Try again
}
void free_hash(hash_t *table)
{
	for (int i=0;i<table->size;i++)
		if (table->mem[i])
			free(table->mem[i]);
	free(table->mem);
	free(table);
}
int main(int argc,char **argv)
{
	hash_t *table=new_hash(1);
	char *input=calloc(100,1),*sym=malloc(100);
	int *num=malloc(sizeof(int));
	while (strcmp(input,"quit\n")&&strcmp(input,"\n")) {
		printf("Command: ");
		fgets(input,99,stdin);
		if (sscanf(input,"add %s %i",sym,num)==2) {
			add_entry(table,sym,num);
			printf("Adding %s as %i\n\n",sym,*num);
			num=malloc(sizeof(int));
		} else if (sscanf(input,"set %s %i",sym,num)==2) {
			set_entry(table,sym,num);
			printf("Setting %s to %i\n\n",sym,*num);
			num=malloc(sizeof(int));
		} else if (sscanf(input,"get %s",sym)==1) {
			int *ptr=get_entry(table,sym);
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
			table->mem=realloc(table->mem,table->size*sizeof(int *));
		} else
			printf("Unrecognized command\n\n");
}
	free(input);
	free(sym);
	free(num);
	free_hash(table);
	return 0;
}
