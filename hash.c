#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	int size; // # of possible entries
	int initsize;
	int **mem; // int could be be changed to anything (even void)
} hash_t;
unsigned long hash_key(char *str) // lazy
{
	unsigned long l;
	for (char *c=str;*c;c++) {
		l<<=1;
		l+=(*c)&31; // case insensitive; suits my needs
	}
	return l;
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
	if (def)
		free(*def);
	*def=entry;
}
void add_entry(hash_t *table,char *str,int *entry) // set_entry but handles collisions
{
	int *def=table->mem[hash_key(str)%table->size];
	if (def) { // Hash collision. Double the memory bank
		table->size*=2;
		table->mem=realloc(table->mem,table->size*sizeof(int *));
	}
	set_entry(table,str,entry);
}
int *get_entry(hash_t *table,char *str)
{
	int size=table->size;
	int *def=table->mem[hash_key(str)%size];
	if (def)
		return def;
	while (!def&&size>table->initsize) // Check possible locations
		def=table->mem[hash_key(str)%(size/=2)];
	if (!def)
		return NULL; // Not defined
	add_entry(table,str,def); // Give it a newer entry
	table->mem[hash_key(str)%table->size]=NULL; // Remove the old one
	return def;
}
void free_hash(hash_t *table)
{
	for (int i=0;i<table->size;i++)
		if (table->mem[i])
			free(table->mem[i]);
	free(table);
}
int main(int argc,char **argv)
{
	hash_t *table=new_hash(128);
	char *input=malloc(100),*sym=malloc(100);
	int *num=malloc(sizeof(int));
	while (strcmp(input,"quit\n")) {
		printf("Command: ");
		fgets(input,99,stdin);
		if (sscanf(input,"add %s %i",sym,num)==2) {
			printf("Adding %s as %i\n",sym,*num);
			add_entry(table,sym,num);
			num=malloc(sizeof(int));
		} else if (sscanf(input,"set %s %i",sym,num)==2) {
			printf("Setting %s to %i\n",sym,*num);
			set_entry(table,sym,num);
			num=malloc(sizeof(int));
		} else if (sscanf(input,"get %s",sym)==1) {
			printf("%i\n",*get_entry(table,sym));
			num=malloc(sizeof(int));
		} else if (!strcmp(input,"size\n"))
			printf("%i possible entries\n",table->size);
	}
	free(num);
	free_hash(table);
	return 0;
}
