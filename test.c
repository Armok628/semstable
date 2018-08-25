#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/hash.h"
#include "src/timer.h"
#include "src/randword.h"
#define WORD_SIZE (10+rand()%11)
typedef enum {false,true} bool;
typedef struct test_s {
	char *str;
	int val;
	struct test_s *cdr;
} test_t;
test_t *new_test()
{
	test_t *t=malloc(sizeof(test_t));
	t->str=random_word(WORD_SIZE);
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
int find_dup_tests(test_t *list,test_t *test)
{
	int i=0;
	for (;list;list=list->cdr)
		i+=!strcmp(list->str,test->str);
	return i-1;

}
void locdump(table_t *table)
{ // Print number of buckets in each location
	for (int i=0;i<table->size;i++) {
		int c=0;
		bucket_t *b=table->pool[i];
		for (;b;b=b->cdr)
			c++;
		printf("%d ",c);
	}
	puts("\n");
}
int main(int argc,char **argv)
{
	if (argc<4)
		goto NOT_ENOUGH_ARGS;
	// Scan arguments
	int tsize=0,words=0,tests=0;
	bool dump=false,time_only=false,m_expunge=false,no_adds=false,print_keys=false;
	unsigned int seed=time(NULL);
	sscanf(argv[1],"%d",&tsize);
	sscanf(argv[2],"%d",&words);
	sscanf(argv[3],"%d",&tests);
	for (int i=1;i<argc;i++) {
		sscanf(argv[i],"size=%d",&tsize);
		sscanf(argv[i],"words=%d",&words);
		sscanf(argv[i],"tests=%d",&tests);
		sscanf(argv[i],"seed=%u",&seed);
		if (!strcmp(argv[i],"--dump"))
			dump=true;
		if (!strcmp(argv[i],"--time_only"))
			time_only=true;
		if (!strcmp(argv[i],"--expunge"))
			m_expunge=true;
		if (!strcmp(argv[i],"--no_adds"))
			no_adds=true;
		if (!strcmp(argv[i],"--print_keys"))
			print_keys=true;
	}
	// Initialize table and test variables
	srand(seed);
	table_t *table=new_table(tsize,&free);
	test_t *testlist=new_test(); // Make first test
	test_t *test=testlist;
	insert(table,test->str,intptr_to(test->val));
	// Build expectations and add table values
	for (int i=1;i<words;i++) {
		test_t *t=new_test();
		if (!time_only&&!no_adds)
			printf("Adding %s as %d\n",t->str,t->val);
		if (print_keys)
			printf("%s -> %lu\n",t->str,fnv_1a(t->str));
		insert(table,t->str,intptr_to(t->val));
		test->cdr=t;
		test=t;
	}
	// Test expectations
	start_timer();
	int successes=0,failures=0,duplicates=0;
	for (int i=0;i<tests;i++) {
		//printf("Test iteration: %d\n",i);
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
				int d=find_dup_tests(testlist,test);
				duplicates+=!!d;
				if (!d) {
					printf("Failure: Wrong value for %s\n",test->str);
					failures++;
				}
			}
		}
	}
	// Print report
	if (!time_only) {
		printf("\nSuccesses: %d\nDuplicates: %d\nFailures: %d\n",successes,duplicates,failures);
		double dur=read_timer();
		printf("\nTotal value retrieval time: %f seconds\n",dur);
		printf("\nAverage: %f ms per retrieval\n\n",dur/(words*tests/1000.0));
	} else
		printf("%lf",read_timer());
	if (dump)
		locdump(table);
	// Manually expunge values if requested
	if (m_expunge) {
		start_timer();
		for (test=testlist;test;test=test->cdr)
			expunge(table,test->str);
		if (!time_only)
			printf("Total data expunge time: %lf seconds\n\n",read_timer());
		if (dump)
			locdump(table);
	}
	if (!time_only)
		printf("Seed: %u\n\n",seed);
	// Clean up
	for (test=testlist;test;) { // Free test memory
		test_t *t=test;
		free(t->str);
		test=t->cdr;
		free(t);
	}
	free_table(table);
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
			} else if (sscanf(input,"fnv_1a %s",str)==1) {
				printf("%s -> %lu\n\n",str,fnv_1a(str));
			} else if (sscanf(input,"new_table %ld",&num)==1) {
				printf("Making new table of size %ld\n\n",num);
				free_table(table);
				table=new_table(num,NULL);
			} else
				printf("Unrecognized command or format\n\n");
		}
		free_table(table);
	}
	return 0;
}
