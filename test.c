#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/hash.h"
#include "src/timer.h"
#include "src/randword.h"
unsigned long hash_key(char *); // Not ordinarily accessible outside hash.c
typedef struct {
	char *str;
	int val;
} test_t;
typedef struct tests_s {
	test_t *test;
	struct tests_s *cdr;
} tests_t;
test_t *new_test()
{
	test_t *t=malloc(sizeof(test_t));
	t->str=random_word(10);
	t->val=rand();
	return t;
}
int *intptr_to(int i)
{
	int *p=malloc(sizeof(int));
	*p=i;
	return p;
}
int main(int argc,char **argv)
{
	if (argc<4)
		goto NOT_ENOUGH_ARGS;
	int tsize=0,words=0,tests=0;
	sscanf(argv[1],"%i",&tsize);
	sscanf(argv[2],"%i",&words);
	sscanf(argv[3],"%i",&tests);
	srand(time(NULL));
	table_t *table=new_table(tsize,&free);
	tests_t *testlist=NULL,*tl=testlist;
	// Build expectations and add table values
	for (int i=0;i<words;i++) {
		test_t *t=new_test();
		add_entry(table,t->str,intptr_to(t->val));
		printf("Adding %s as %i\n",t->str,t->val);
		//fprintf(stderr,"%s -> %lu\n",t->str,hash_key(t->str));
		if (testlist) {
			tl->cdr=malloc(sizeof(tests_t));
			tl=tl->cdr;
			tl->test=t;
			tl->cdr=NULL;
		} else { // Lazy hack
			testlist=malloc(sizeof(tests_t));
			testlist->test=t;
			testlist->cdr=NULL;
			tl=testlist;
		}
	}
	// Test expectations
	start_timer();
	int successes=0,failures=0;
	for (int i=0;i<tests;i++) {
		//printf("Test iteration: %i\n",i);
		for (tl=testlist;tl;tl=tl->cdr) {
			test_t *t=tl->test;
			//printf("Test: Getting entry for %s\n",t->str);
			int *ptr=(int *)get_entry(table,t->str);
			if (!ptr) {
				printf("Failure: get_entry() returned null\n");
				failures++;
				continue;
			}
			if (*ptr==t->val) {
				//printf("Success!\n");
				successes++;
			} else {
				printf("Failure: Wrong value for %s\n",t->str);
				failures++;
			}
		}
	}
	fprintf(stderr,"\nSuccesses: %i\nFailures: %i\n",successes,failures);
	fprintf(stderr,"\nTotal value retrieval time: %lf seconds\n\n",read_timer());
	for (tl=testlist;tl;) { // Free test memory
		test_t *t=tl->test;
		free(t->str);
		free(t);
		tests_t *f=tl;
		tl=tl->cdr;
		free(f);
	}
	free_table(table); // Free table and entries
	return 0;
NOT_ENOUGH_ARGS:
	table=new_table(128,NULL);
	if (argc<2) {
		char input[100],sym[100];
		long num;
		for (;;) {
			printf("Command: ");
			fgets(input,99,stdin);
			if (!strcmp(input,"quit\n")||(*input=='\n'))
				break;
			else if (sscanf(input,"add %s %ld",sym,&num)==2) {
				printf("Adding %s as %ld\n\n",sym,num);
				add_entry(table,sym,(void *)num);
			} else if (sscanf(input,"set %s %ld",sym,&num)==2) {
				printf("Setting %s to %ld\n\n",sym,num);
				set_entry(table,sym,(void *)num);
			} else if (sscanf(input,"get %s",sym)==1) {
				long n=(long)get_entry(table,sym);
				if (n)
					printf("%ld\n\n",n);
				else
					printf("%s is not defined or is zero\n\n",sym);
			} else if (sscanf(input,"key %s",sym)==1) {
				printf("%s -> %lu\n\n",sym,hash_key(sym));
			} else if (!strcmp(input,"realloc\n")) {
				table->size+=table->size+1;
				table->mem=realloc(table->mem,table->size*sizeof(bucket_t *));
			} else
				printf("Unrecognized command or format\n\n");
		}
		free_table(table);
	}
	return 0;
}
