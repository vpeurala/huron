#include "huron/object.h"
#include "huron/gc.h"

#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

static void die(const char *s)
{
	fprintf(stderr, "fatal: %s\n", s);
	exit(1);
}

static void gc_torture(void)
{
	int i;

	for (i = 0; i < 10000; i++) {
		void *p;

		p = huron_object_new(HURON_OBJECT_ATOM);
		if (!p)
			die("out of memory");
	}
}

int main(void)
{
	huron_gc_init();

	puts("Huron, version 0.1");

	for (;;) {
		const char *line;

		line = readline("> ");
		if (!line)
			break;

		if (!strncmp(line, ":gc-torture", strlen(":gc-torture")))
			gc_torture();
	}

	printf("\nLeaving Huron.\n");

	return 0;
}
