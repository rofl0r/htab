#ifndef HSEARCH_H
#define HSEARCH_H

#include <stdlib.h>

typedef enum { FIND, ENTER } ACTION;

typedef struct entry {
	char *key;
	void *data;
} ENTRY;

struct elem {
	ENTRY item;
	size_t hash;
};

struct htab {
	struct elem *elems;
	size_t mask;
	size_t used;
};

int htab_create(struct htab *, size_t);
void htab_hdestroy(struct htab *);
int htab_hsearch(struct htab *, ENTRY, ACTION, ENTRY **);

#endif
