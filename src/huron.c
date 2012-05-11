#include "huron/object.h"
#include "huron/gc.h"

#include <stdlib.h>
#include <stdio.h>

static void die(const char *s)
{
	fprintf(stderr, "fatal: %s\n", s);
	exit(1);
}

int main(void)
{
	int i;

	huron_gc_init();

	for (i = 0; i < 10000; i++) {
		void *p;

		p = huron_object_new(HURON_OBJECT_ATOM);
		if (!p)
			die("out of memory");
	}

	return 0;
}
