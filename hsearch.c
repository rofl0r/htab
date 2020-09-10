/*
musl license, hsearch.c originally written by Szabolcs Nagy

Copyright © 2005-2020 Rich Felker, et al.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdlib.h>
#include <string.h>
#include "hsearch.h"

/*
open addressing hash table with 2^n table size
quadratic probing is used in case of hash collision
tab indices and hash are size_t
after resize fails with ENOMEM the state of tab is still usable

with the posix api items cannot be iterated and length cannot be queried
*/

#define MINSIZE 8
#define MAXSIZE ((size_t)-1/2 + 1)

static size_t keyhash(char *k)
{
	unsigned char *p = (void *)k;
	size_t h = 0;

	while (*p)
		h = 31*h + *p++;
	return h;
}

static int resize(size_t nel, struct hsearch_data *htab)
{
	size_t newsize;
	size_t i, j;
	struct elem *e, *newe;
	struct elem *oldtab = htab->elems;
	struct elem *oldend = htab->elems + htab->mask + 1;

	if (nel > MAXSIZE)
		nel = MAXSIZE;
	for (newsize = MINSIZE; newsize < nel; newsize *= 2);
	htab->elems = calloc(newsize, sizeof *htab->elems);
	if (!htab->elems) {
		htab->elems = oldtab;
		return 0;
	}
	htab->mask = newsize - 1;
	if (!oldtab)
		return 1;
	for (e = oldtab; e < oldend; e++)
		if (e->item.key) {
			for (i=e->hash,j=1; ; i+=j++) {
				newe = htab->elems + (i & htab->mask);
				if (!newe->item.key)
					break;
			}
			*newe = *e;
		}
	free(oldtab);
	return 1;
}

static struct elem *lookup(char *key, size_t hash, struct hsearch_data *htab)
{
	size_t i, j;
	struct elem *e;

	for (i=hash,j=1; ; i+=j++) {
		e = htab->elems + (i & htab->mask);
		if (!e->item.key ||
		    (e->hash==hash && strcmp(e->item.key, key)==0))
			break;
	}
	return e;
}

int hcreate_r(size_t nel, struct hsearch_data *htab)
{
	int r;

	r = resize(nel, htab);

	return r;
}

void hdestroy_r(struct hsearch_data *htab)
{
	free(htab->elems);
}

int hsearch_r(ENTRY item, ACTION action, ENTRY **retval, struct hsearch_data *htab)
{
	size_t hash = keyhash(item.key);
	struct elem *e = lookup(item.key, hash, htab);

	if (e->item.key) {
		*retval = &e->item;
		return 1;
	}
	if (action == FIND) {
		*retval = 0;
		return 0;
	}
	e->item = item;
	e->hash = hash;
	if (++htab->used > htab->mask - htab->mask/4) {
		if (!resize(2*htab->used, htab)) {
			htab->used--;
			e->item.key = 0;
			*retval = 0;
			return 0;
		}
		e = lookup(item.key, hash, htab);
	}
	*retval = &e->item;
	return 1;
}
