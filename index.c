#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
char *strclone(char *src)
{
	char *dest=calloc(strlen(src)+1,1);
	strcpy(dest,src);
	return dest;
}
void free_tablebucket(bucket_t *b)
{
	free_table(b->val);
	if (b->cdr)
		free_tablebucket(b->cdr);
	free(b);
}
void free_tabletable(table_t *table)
{
	for (int i=0;i<table->size;i++)
		if (table->mem[i])
			free_tablebucket(table->mem[i]);
	free(table->mem);
	free(table);
}
int main(int argc,char **argv)
{
	table_t *book=new_table(10);
	char *input=calloc(201,1),*arg1=calloc(201,1),*arg2=calloc(201,1),*arg3=calloc(201,1);
	for (;;) {
		fgets(input,200,stdin);
		if (!strcmp(input,"quit\n"))
			break;
		else if (sscanf(input,"set %s %s %s",arg1,arg2,arg3)==3) {
			printf("Setting %s's %s to %s...\n",arg1,arg2,arg3);
			table_t *person=(table_t *)get_entry(book,arg1);
			if (!person) {
				person=new_table(10);
				add_entry(book,arg1,person);
			}
			char *entry=(char *)get_entry(person,arg2);
			if (!entry)
				add_entry(person,arg2,strclone(arg3));
			else {
				free(entry);
				set_entry(person,arg2,strclone(arg3));
			}
		} else if (sscanf(input,"get %s %s",arg1,arg2)==2) {
			table_t *person=(table_t *)get_entry(book,arg1);
			if (!person) {
				printf("%s does not have any entries\n",arg1);
				continue;
			}
			char *entry=(char *)get_entry(person,arg2);
			if (!entry) {
				printf("%s does not have %s\n",arg1,arg2);
				continue;
			}
			puts(entry);
		} else
			printf("Unrecognized command format\n");
	}
	free(input);
	free(arg1);
	free(arg2);
	free(arg3);
	free_tabletable(book); // Bug: Strings stored in inner tables are not freed
}
