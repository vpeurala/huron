#ifndef HURON_OBJECT_H
#define HURON_OBJECT_H

/*
 * Atoms
 */
struct huron_atom {
	char				*ident;
	struct huron_object		*value;
};

/*
 * Cons pairs
 */
struct huron_cons {
	struct huron_object		*car;
	struct huron_object		*cdr;
};

enum huron_object_type {
	HURON_OBJECT_ATOM,
	HURON_OBJECT_CONS,
};

struct huron_object {
	unsigned long			type;		/* see "enum huron_object_type" */
	union {
		struct huron_atom	atom;
		struct huron_cons	cons;
	} value;
	unsigned long			padding;
};

struct huron_object *huron_object_new(enum huron_object_type type);

#endif /* HURON_OBJECT_H */
