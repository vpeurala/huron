#include "huron/eval.h"

#include "huron/object.h"

#include <stdlib.h>
#include <stddef.h>

struct huron_object *huron_eval(const char *s)
{
	struct huron_object *obj;

	obj = huron_object_new(HURON_OBJECT_NUMBER);
	if (!obj)
		return NULL;

	obj->value.number = strtoll(s, NULL, 10);

	return obj;
}
