#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	int size; // # of possible entries
	int initsize;
	int **mem; // int could be be changed to anything (even void)
} hash_t;
unsigned long hash_key(char *str) // long long could be appropriate
{
	unsigned long l;
	for (char *c=str;*c;c++) {
		l<<=4;
		l+=*c&15; // four lowest bits -> case insensitive
	}
	return l;
}
hash_t *new_table(int size) {
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
	if (def) { // Hash collision. Double the memory bank to avoid
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
	for (int i=0;i<table->size/sizeof(int *);i++)
		if (table->mem[i])
			free(table->mem[i]);
	free(table);
}
int main(int argc,char **argv)
{
	hash_t *table=new_table(128);
	char *input=malloc(100),*number=malloc(100);
	while (strcmp(input,"quit\n")) {
		printf("Command: ");
		fgets(input,99,stdin);
		if (!strcmp(input,"set\n")) {
			printf("Set what? ");
			fgets(input,99,stdin);
			input[strlen(input)-1]='\0';
			printf("...to what? ");
			fgets(number,99,stdin);
			number[strlen(number)-1]='\0';
			int *num=malloc(sizeof(int));
			if (sscanf(number,"%i",num)) {
				printf("Setting %s to %i\n",input,*num);
				set_entry(table,input,num);
			}
		} else if (!strcmp(input,"add\n")) {
			printf("Add what? ");
			fgets(input,99,stdin);
			input[strlen(input)-1]='\0';
			printf("...as what? ");
			fgets(number,99,stdin);
			number[strlen(number)-1]='\0';
			int *num=malloc(sizeof(int));
			if (sscanf(number,"%i",num)) {
				printf("Adding %s as %i\n",input,*num);
				add_entry(table,input,num);
			}
		} else if (!strcmp(input,"get\n")) {
			printf("Get what? ");
			input=fgets(input,99,stdin);
			input[strlen(input)-1]='\0';
			int *p=get_entry(table,input);
			if (!p)
				printf("%s is not defined\n",input);
			else
				printf("%i\n",*p);
		} else if (!strcmp(input,"size\n")) {
			printf("Current size: %i possible entries\n",table->size);
		} else {
			char *sym=malloc(50);
			int *num=malloc(sizeof(int));
			if (sscanf(input,"add %s %i",sym,num)==2) {
				printf("Adding %s as %i\n",sym,*num);
				add_entry(table,sym,num);
			} else if (sscanf(input,"set %s %i",sym,num)==2) {
				printf("Setting %s to %i\n",sym,*num);
				set_entry(table,sym,num);
			} else if (sscanf(input,"get %s",sym)==1) {
				printf("%i\n",*get_entry(table,sym));
			} else {
				free(sym);
				free(num);
			}
		}

	}
	free_hash(table);
	return 0;
}
