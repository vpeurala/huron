#include "huron/object.h"
#include "huron/eval.h"
#include "huron/gc.h"

#include <stdio.h>
#include <readline/readline.h>

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

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

static void print(struct huron_object *obj)
{
	if (!obj) {
		puts("nil");
		return;
	}
	switch (obj->type) {
	case HURON_OBJECT_NUMBER:
		printf("%" PRId64 "\n", obj->value.number);
		break;
	}
}

int main(void)
{
	huron_gc_init();

	puts("Huron, version 0.1");

	for (;;) {
		struct huron_object *result;
		const char *line;

		line = readline("> ");
		if (!line)
			break;

		if (strlen(line) == 0)
			continue;

		if (!strncmp(line, ":gc-torture", strlen(":gc-torture"))) {
			gc_torture();
			continue;
		}

		result = huron_eval(line);

		print(result);
	}

	printf("\nLeaving Huron.\n");

	return 0;
}
