#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hsearch.h"

static void put_mark (void) {
	fputc ('#', stdout);
	fflush (stdout);
}

static void maybe_put_mark (int n_ops) {
	if (n_ops > 0 && !(n_ops % 1000))
		put_mark ();
}

int main() {
	struct htab *h = htab_create(16);
	assert(h);

	size_t i = 0;
	char *k;
	htab_value *v;
	while(i=htab_next(h, i, &k, &v)) {
		printf("XXXX shouldnt happen %s -> %zu\n", k, v->n);
	}

	assert(htab_insert(h, "foo", HTV_N(42)));
	assert(htab_insert(h, "bar", HTV_N(31337)));
	assert(htab_insert(h, "baz", HTV_P("a string")));
	assert(htab_insert(h, "qux", HTV_N(64)));
	assert(htab_insert(h, "900", HTV_N(900)));
	assert(htab_insert(h, "112", HTV_N(112)));

	v= htab_find(h, "bar");
	assert(v);
	printf("%zu\n", v->n);

	assert(0 != htab_find(h, "baz"));
	assert(0 != htab_delete(h, "baz"));
	assert(0 == htab_find(h, "baz"));

	i = 0;
	while(i=htab_next(h, i, &k, &v)) {
		printf("%s -> %zu\n", k, v->n);
		/* change value of stored item */
		if(!strcmp(k, "112")) v->n++;
	}
	v = htab_find(h, "112");
	assert(v && v->n == 113);

	htab_destroy(h);

#define N_TESTS 1000000

#ifdef SHOOTOUT
#define STRDUP(X) X
#else
#define STRDUP(X) strdup(X)
#endif

	h = htab_create(N_TESTS*1.3);
	assert(h);
	for(i=0;i<N_TESTS;i++) {
		char buf[32];
		snprintf(buf, sizeof buf, "mystring-%zu", i);
		assert(htab_insert(h, STRDUP(buf), HTV_N(i)));
		maybe_put_mark(i);
	}
	puts("");

#ifndef SHOOTOUT
	/* "sequentialstring" shootout benchmark neither uses strdup to
	   insert strings, nor does it access them afterwards. */
	for(i=0;i<N_TESTS;i++) {
		char buf[32];
		snprintf(buf, sizeof buf, "mystring-%zu", i);
		v = htab_find(h, buf);
		assert(v && v->n == i);
	}
	while(i=htab_next(h, i, &k, &v)) {
		free(k);
	}
#endif
	htab_destroy(h);
}
