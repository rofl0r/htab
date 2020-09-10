#ifndef HSEARCH_H
#define HSEARCH_H

#include <stdlib.h>

typedef enum { FIND, ENTER } ACTION;

typedef union htab_value {
	void *p;
	size_t n;
} htab_value;

typedef struct htab_entry {
	char *key;
	htab_value data;
} htab_entry;

struct elem {
	htab_entry item;
	size_t hash;
};

struct htab {
	struct elem *elems;
	size_t mask;
	size_t used;
};

struct htab * htab_create(size_t);
void htab_hdestroy(struct htab *);
htab_value* htab_find(struct htab *, char* key);
htab_entry* htab_hsearch(struct htab *, htab_entry, ACTION);

#endif
