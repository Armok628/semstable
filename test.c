#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/hash.h"
#include "src/timer.h"
#include "src/randword.h"
unsigned long hash_key(char *); // Not ordinarily accessible outside hash.c
typedef struct test_s {
	char *str;
	int val;
	struct test_s *cdr;
} test_t;
test_t *new_test()
{
	test_t *t=malloc(sizeof(test_t));
	t->str=random_word(10);
	t->val=rand();
	t->cdr=NULL;
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
	test_t *testlist=new_test(); // Make first test
	test_t *test=testlist;
	insert(table,test->str,intptr_to(test->val));
	// Build expectations and add table values
	for (int i=1;i<words;i++) {
		test_t *t=new_test();
		insert(table,t->str,intptr_to(t->val));
		printf("Adding %s as %i\n",t->str,t->val);
		//fprintf(stderr,"%s -> %lu\n",t->str,hash_key(t->str));
		test->cdr=t;
		test=t;
	}
	// Test expectations
	start_timer();
	int successes=0,failures=0;
	for (int i=0;i<tests;i++) {
		//printf("Test iteration: %i\n",i);
		for (test=testlist;test;test=test->cdr) {
			//printf("Test: Getting entry for %s\n",t->str);
			int *ptr=(int *)lookup(table,test->str);
			if (!ptr) {
				printf("Failure: lookup returned null\n");
				failures++;
			} else if (*ptr==test->val) {
				//printf("Success!\n");
				successes++;
			} else {
				printf("Failure: Wrong value for %s\n",test->str);
				failures++;
			}
		}
	}
	fprintf(stderr,"\nSuccesses: %i\nFailures: %i\n",successes,failures);
	fprintf(stderr,"\nTotal value retrieval time: %lf seconds\n\n",read_timer());
	for (test=testlist;test;) { // Free test memory
		test_t *t=test;
		free(t->str);
		test=t->cdr;
		free(t);
	}
	free_table(table); // Free table and entries
	return 0;
NOT_ENOUGH_ARGS:
	table=new_table(128,NULL);
	if (argc<2) {
		char input[100],str[100];
		long num;
		for (;;) {
			printf("Command: ");
			fgets(input,99,stdin);
			if (!strcmp(input,"quit\n")||(*input=='\n'))
				break;
			else if (sscanf(input,"insert %s %ld",str,&num)==2) {
				printf("Inserting %s as %ld\n\n",str,num);
				insert(table,str,(void *)num);
			} else if (sscanf(input,"lookup %s",str)==1) {
				long n=(long)lookup(table,str);
				if (n)
					printf("%ld\n\n",n);
				else
					printf("%s is undefined or zero\n\n",str);
			} else if (sscanf(input,"expunge %s",str)==1) {
				printf("Expunging %s\n\n",str);
				expunge(table,str);
			} else if (sscanf(input,"hash_key %s",str)==1) {
				printf("%s -> %lu\n\n",str,hash_key(str));
			} else
				printf("Unrecognized command or format\n\n");
		}
		free_table(table);
	}
	return 0;
}
