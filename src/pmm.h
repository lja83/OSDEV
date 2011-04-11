#ifndef PMM_H_
#define PMM_H_

#include "common.h"

typedef struct
{
	u32int *start_addr;
	u32int length;
	int available;
} memory_region_t;

u32int *palloc();
u32int pfree(u32int *page_address);
void init_pmm(u32int *map_address, memory_region_t regions[]);

#endif /* PMM_H_ */
