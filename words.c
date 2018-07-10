#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/hash.h"
#include "src/timer.h"
void chomp(char *s)
{ // Remove newline at string end
	for (;*s;s++);
	if (s[-1]=='\n')
		s[-1]='\0';
}
int main(int argc,char **argv)
{
	char buf[100]="/usr/share/dict/words",*s=NULL;
	int count=0,failures=0,table_size=125; // 1k by default
	FILE *words=NULL;
	for (int i=1;i<argc;i++) {
		sscanf(argv[i],"words=%s",buf);
		sscanf(argv[i],"size=%d",&table_size);
	}
	words=fopen(buf,"r");
	if (!words) {
		printf("Unable to access words file at %s\n",buf);
		return 1;
	}
	table_t *dict=new_table(table_size,&free);
	while (fgets(buf,100,words)) {
		chomp(buf);
		printf("Inserting %s\n",buf);
		insert(dict,buf,strdup(buf));
		count++;
	}
	fseek(words,0,SEEK_SET);
	start_timer();
	while (fgets(buf,100,words)) {
		chomp(buf);
		if (strcmp(buf,(s=lookup(dict,buf)))) {
			printf("Collision: '%s' with '%s'\n",buf,s);
			failures++;
		}
	}
	printf("\nSuccessfully checked %d/%d words\n",count-failures,count);
	printf("Collision check phase took %lf seconds\n",read_timer());
	printf("Lookup table size: %lu bytes\n\n",table_size*sizeof(bucket_t *));
	fclose(words);
	free_table(dict);
}
