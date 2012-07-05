#include "huron/object.h"
#include "huron/eval.h"
#include "huron/expr.h"
#include "huron/jit.h"
#include "huron/gc.h"

#include <stdio.h>
#include <readline/readline.h>

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int huron_debug;
int huron_jit;

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

static void parse_args(int argc, char *argv[])
{
	int c;

	while ((c = getopt(argc, argv, "dj")) != -1) {
		switch (c) {
		case 'd':
			huron_debug	= 1;
			break;
		case 'j':
			huron_jit	= 1;
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	parse_args(argc, argv);

	if (huron_jit)
		huron_jit_init();

	huron_gc_init();

	puts("Huron, version 0.1");

	for (;;) {
		struct huron_object *result;
		struct huron_object *expr;
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

		expr = huron_parse(line);

		if (huron_jit) {
			struct huron_function *function;

			function = huron_function_compile(expr);

			result = huron_function_call(function);

			huron_function_delete(function);
		} else {
			result = huron_eval(expr);
		}

		print(result);
	}

	printf("\nLeaving Huron.\n");

	return 0;
}
