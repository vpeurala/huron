#include "huron/expr.h"

#include "huron/object.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static struct huron_object *huron_number_new(int64_t value)
{
	struct huron_object *obj;

	obj = huron_object_new(HURON_OBJECT_NUMBER);
	if (!obj)
		return NULL;

	obj->value.number = value;

	return obj;
}

struct huron_object *huron_parse(const char *s)
{
	unsigned long value;

	value = strtoll(s, NULL, 10);

	return huron_number_new(value);
}
