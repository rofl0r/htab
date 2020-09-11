#ifndef HSEARCH_H
#define HSEARCH_H

#include <stdlib.h>

typedef union htab_value {
	void *p;
	size_t n;
} htab_value;

struct htab * htab_create(size_t);
void htab_destroy(struct htab *);
htab_value* htab_find(struct htab *, char* key);
int htab_insert(struct htab *, char*, htab_value);

#endif
