#include "huron/gc.h"

#include "huron/object.h"

#include <strings.h>	/* for ffsl() */

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))

#define KB(n)		(1024UL * n)

#define BITS_PER_BYTE	8
#define BITS_PER_LONG	(sizeof(unsigned long) * 8)

/*
 * Heap size must be multiple of sizeof(unsigned long)
 */
#define HEAP_SIZE	KB(2)

#define GC_OBJ_SIZE	sizeof(struct huron_object)

static char		gc_heap[HEAP_SIZE];

#define			GC_BITMAP_BYTES (HEAP_SIZE / GC_OBJ_SIZE)

#define			GC_BITMAP_SIZE (BITS_PER_BYTE * GC_BITMAP_BYTES / BITS_PER_LONG)

static unsigned long	gc_freelist[GC_BITMAP_SIZE];

static unsigned long	gc_inuse_list[GC_BITMAP_SIZE];

static bool		huron_verbose_gc = true;

static inline unsigned long bit_mask(unsigned long bit)
{
	return 1UL << (bit & (BITS_PER_LONG-1));
}

void set_bit(unsigned long *bitset, unsigned long bit)
{
	unsigned long *addr, mask;

	addr = bitset + (bit / BITS_PER_LONG);
	mask = bit_mask(bit);

	*addr |= mask;
}

void clear_bit(unsigned long *bitset, unsigned long bit)
{
	unsigned long *addr, mask;

	addr = bitset + (bit / BITS_PER_LONG);
	mask = bit_mask(bit);

	*addr &= ~mask;
}

static inline int test_bit(unsigned long *bitset, unsigned long bit)
{
	unsigned long *addr, mask;

	addr = bitset + (bit / BITS_PER_LONG);
	mask = bit_mask(bit);

	return ((*addr & mask) != 0);
}

static unsigned long
bitmap_ffs(unsigned long *bitmap, size_t size)
{
	unsigned long i;

	for (i = 0; i < size; i++) {
		unsigned long idx;

		idx = ffsl(bitmap[i]);
		if (!idx)
			continue;

		return i * BITS_PER_LONG + idx;
	}

	return 0;
}

static unsigned long
bitmap_find_next_zero(unsigned long *bitmap, size_t size, unsigned long start)
{
	unsigned long idx;

	for (idx = start; idx < size * BITS_PER_LONG; idx++) {
		if (!test_bit(bitmap, idx)) {
			return idx;
		}
	}

	return size * BITS_PER_LONG;
}

#define for_each_zero_bit(idx, bitmap, size)				\
	for (idx = bitmap_find_next_zero(bitmap, size, 0);		\
	     idx < size * BITS_PER_LONG;				\
	     idx = bitmap_find_next_zero(bitmap, size, idx + 1))	\

static inline void *obj_index_to_ptr(unsigned long idx)
{
	return (void *) gc_heap + (idx - 1) * GC_OBJ_SIZE;
}

static void
obj_mark_used(unsigned long idx)
{
	clear_bit(gc_freelist, idx - 1);
}

static void
obj_mark_unused(unsigned long idx)
{
	set_bit(gc_freelist, idx - 1);
}

bool huron_gc_collect(void)
{
	unsigned long idx, nr = 0;

	memset(gc_inuse_list, 0, GC_BITMAP_BYTES);

	for_each_zero_bit(idx, gc_inuse_list, GC_BITMAP_SIZE) {
		obj_mark_unused(idx + 1);
		nr++;
	}

	if (huron_verbose_gc)
		printf("[GC reclaimed %lu objects]\n", nr);

	return nr > 0;
}

void *huron_gc_object_alloc(void)
{
	unsigned long idx;

retry:
	idx = bitmap_ffs(gc_freelist, ARRAY_SIZE(gc_freelist));
	if (!idx) {
		if (!huron_gc_collect())
			return NULL;

		goto retry;
	}

	obj_mark_used(idx);

	return obj_index_to_ptr(idx);
}

void huron_gc_init(void)
{
	memset(gc_freelist, 0xff, GC_BITMAP_BYTES);
}
