#ifndef HSEARCH_H
#define HSEARCH_H

typedef enum { FIND, ENTER } ACTION;

typedef struct entry {
	char *key;
	void *data;
} ENTRY;

int hcreate(size_t);
void hdestroy(void);
ENTRY *hsearch(ENTRY, ACTION);

struct hsearch_data {
	struct tab *tab;
};

int hcreate_r(size_t, struct hsearch_data *);
void hdestroy_r(struct hsearch_data *);
int hsearch_r(ENTRY, ACTION, ENTRY **, struct hsearch_data *);

#endif
