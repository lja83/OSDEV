#ifndef KHEAP_H_
#define KHEAP_H_

#define KHEAP_START				0xC0000000
#define KHEAP_INITIAL_SIZE		0x100000
#define HEAP_INDEX_SIZE			0x20000
#define HEAP_MAGIC				0x123890AB
#define HEAP_MIN_SIZE			0x70000

#include "common.h"
#include "ordered_array.h"

typedef struct
{
	u32int magic;			// Magic number, used for error checking and identification
	u8int is_hole;			// 1 if this is a hole, 0 if this is a block.
	u32int size;			// Size of the block, including this and the footer
} header_t;

typedef struct
{
	u32int magic;			// Magic number, same as in header_t
	header_t *header;		// Pointer to the block header
} footer_t;

typedef struct
{
	ordered_array_t index;
	u32int start_address;		// The start of our allocated space.
	u32int end_address;			// The end of our allocated space. May be expanded up to max_address.
	u32int max_address;			// The maximum address the heap can be expanded to.
	u8int supervisor;			// Should extra pages requested by us be mapped as supervisor-only?
	u8int readonly;				// Should extra pages requested by us be mapped as read-only?
} heap_t;

// Create a new heap.
heap_t *create_heap(u32int start, u32int end, u32int max, u8int supervisor, u8int readonly);

// Allocates a contiguous region of memory 'size' in size. If page_align == 1, it creates that block start
// on a page boundary.
void *alloc(u32int size, u8int page_align, heap_t *heap);

// Releases a block allocated with 'alloc'.
void free(void *p, heap_t *heap);

u32int kmalloc(u32int sz);
u32int kmalloc_a(u32int sz);
u32int kmalloc_p(u32int sz, u32int *phys);
u32int kmalloc_ap(u32int sz, u32int *phys);
u32int kmalloc_int(u32int sz, int align, u32int *phys);

#endif /* KHEAP_H_ */
