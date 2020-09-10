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

int hcreate_r(size_t, struct hsearch_data *);
void hdestroy_r(struct hsearch_data *);
int hsearch_r(ENTRY, ACTION, ENTRY **, struct hsearch_data *);

#endif
