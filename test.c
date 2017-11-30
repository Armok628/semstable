#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
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
