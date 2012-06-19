#ifndef HURON_EVAL_H
#define HURON_EVAL_H

#include <stdint.h>

struct huron_object;

struct huron_object *huron_number_new(int64_t value);
struct huron_object *huron_eval(const char *s);

#endif /* HURON_EVAL_H */
