// Lazy header format, sorry.
// This file is not intended to be used outside of the test program anyway.
char *random_word(int);
#ifndef RANDWORD_H
#define RANDWORD_H
#include <stdlib.h>
#include <string.h>
static char *consonants[33]={
	"b","c","d","f","g","h","j","k","l","m","n","p","qu","r","s","t","v","w","x","y","z"
		,"sh","ch","zh","ng","fr","st","sp","sk","tr","kr","fl","gn"};
static char *vowels[36]={
	"a","e","i","o","u","y"
		,"aa","ae","ai","ao","au","ay"
		,"ea","ee","ei","eo","eu","ey"
		,"ia","ie","ii","io","iu","iy"
		,"oa","oe","oi","oo","ou","oy"
		,"ua","ue","ui","uo","uu","uy"};
char *random_word(int length)
{
	char *word=calloc(2*length+1,1);
	word[0]='\0';
	int vowel_start=rand()%2;
	for (int i=0;i<length;i++) {
		if (vowel_start^(i%2))
			strcat(word,vowels[rand()%36]);
		else
			strcat(word,consonants[rand()%33]);
	}
	word=realloc(word,strlen(word)+1);
	return word;
}
#endif
