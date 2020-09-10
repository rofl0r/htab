#ifndef HSEARCH_H
#define HSEARCH_H

#include <stdlib.h>

typedef enum { FIND, ENTER } ACTION;

typedef struct htab_entry {
	char *key;
	void *data;
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
htab_entry* htab_hsearch(struct htab *, htab_entry, ACTION);

#endif
