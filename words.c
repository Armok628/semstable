#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/hash.h"
#include "src/timer.h"
typedef enum {false,true} bool;
void chomp(char *s)
{ // Remove newline at string end
	for (;*s;s++);
	if (s[-1]=='\n')
		s[-1]='\0';
}
int main(int argc,char **argv)
{
	char buf[100]="/usr/share/dict/words",*s=NULL;
	int table_size=125; // 1k by default
	int count=0,failures=0;
	bool no_adds=false,progress=false;
	FILE *words=NULL;
	for (int i=1;i<argc;i++) {
		sscanf(argv[i],"words=%s",buf);
		sscanf(argv[i],"size=%d",&table_size);
		if (!strcmp(argv[i],"--progress"))
			progress=true;
		if (!strcmp(argv[i],"--no_adds"))
			no_adds=true;
	}
	words=fopen(buf,"r");
	if (!words) {
		printf("Unable to access words file at %s\n",buf);
		return 1;
	}
	table_t *dict=new_table(table_size,&free);
	while (fgets(buf,100,words)) {
		chomp(buf);
		if (!no_adds)
			printf("Inserting %s\n",buf);
		insert(dict,buf,strdup(buf));
		count++;
	}
	fseek(words,0,SEEK_SET);
	if (progress)
		printf("\n\x1b[?25l\x1b[s       / %d",count);
	start_timer();
	for (int i=0;fgets(buf,100,words);i++) {
		if (progress)
			printf("\x1b[u%6d",i);
		chomp(buf);
		if (strcmp(buf,(s=lookup(dict,buf)))) {
			printf("Collision: \"%s\" with \"%s\"\n",buf,s);
			failures++;
		}
	}
	if (progress)
		printf("\x1b[?25h\n");
	printf("\nSuccessfully checked %d/%d words\n",count-failures,count);
	printf("Collision check phase took %lf seconds\n",read_timer());
	printf("Lookup table size: %lu bytes\n\n",table_size*sizeof(bucket_t *));
	fclose(words);
	free_table(dict);
}
