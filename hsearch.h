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

struct hsearch_data {
	struct elem *elems;
	size_t mask;
	size_t used;
};

int htab_create(size_t, struct hsearch_data *);
void htab_hdestroy(struct hsearch_data *);
int htab_hsearch(ENTRY, ACTION, ENTRY **, struct hsearch_data *);

#endif
