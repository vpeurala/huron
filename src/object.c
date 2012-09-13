#include "huron/object.h"

#include "huron/gc.h"

struct huron_object *huron_object_new(enum huron_object_type type)
{
    struct huron_object *ret;

    ret = huron_gc_object_alloc();
    if (ret)
        ret->type = type;

    return ret;
}
