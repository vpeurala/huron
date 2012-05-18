#ifndef HURON_GC_H
#define HURON_GC_H

#include <stdbool.h>

void *huron_gc_object_alloc(void);
void huron_gc_init(void);
bool huron_gc_collect(void);

#endif /* HURON_GC_H */
