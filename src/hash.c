#include "hash.h"
unsigned long hash_key(char *str)
{
	unsigned long key=0;
	for (char *c=str;*c;c++) {
		key+=*c;
		key*=*c;
		key<<=8;
		key/=*c;
	}
	//fprintf(stderr,"-- Key: %s -> %lu --\n",str,key);
	return key;
}
bucket_t* new_bucket(unsigned long key,void *val)
{
	bucket_t *b=malloc(sizeof(bucket_t));
	b->key=key;
	b->val=val;
	b->cdr=NULL;
	return b;
}
void free_buckets(bucket_t *b,void (*d)(void *))
{
	if (d)
		d(b->val);
	if (b->cdr)
		free_buckets(b->cdr,d);
	free(b);
}
table_t *new_table(int size,void (*d)(void *))
{
	table_t *table=malloc(sizeof(table_t));
	table->mem=calloc(size,sizeof(bucket_t *));
	table->size=size;
	table->destructor=d;
	return table;
}
void free_table(table_t *table)
{
	for (int i=0;i<table->size;i++)
		if (table->mem[i])
			free_buckets(table->mem[i],table->destructor);
	free(table->mem);
	free(table);
}
bucket_t *add_bucket(table_t *table,bucket_t *entry)
{
	unsigned long key=entry->key;
	bucket_t **def=&table->mem[key%table->size];
	if (*def) {
		bucket_t *d=*def;
		for (;d->cdr&&d->key!=key;d=d->cdr);
		if (d->key==key) {
			//fprintf(stderr,"-- Key already exists in table --\n");
			if (d->val&&table->destructor)
				table->destructor(d->val);
			d->val=entry->val;
			free(entry);
		} else
			d->cdr=entry;
		def=&d;
	} else
		*def=entry;
	return *def;
}
bucket_t *add_entry(table_t *table,char *str,void *entry)
{
	unsigned long key=hash_key(str);
	bucket_t *bucket=new_bucket(key,entry);
	return add_bucket(table,bucket);
}
void *get_entry(table_t *table,char *str)
{
	unsigned long key=hash_key(str);
	bucket_t *def=table->mem[key%table->size];
	for (;def&&def->key!=key;def=def->cdr);
	if (!def) {
		fprintf(stderr,"-- Entry could not be found --\n");
		return NULL;
	}
	return def->val;
}
void set_entry(table_t *table,char *str,void *entry) // add_entry() without collision handling
{
	unsigned long key=hash_key(str);
	bucket_t *def=table->mem[key%table->size];
	for (;def&&def->key!=key;def=def->cdr);
	if (def) {
		if (table->destructor)
			table->destructor(def->val);
		def->val=entry;
	}
}
