#include "pmm.h"
#include "common.h"
#include "bitset.h"
#include "monitor.h"
#include "string.h"

bitset_t mmap;

u32int *get_page_address(u32int index);
u32int get_page_index(u32int *address);

void init_pmm(u32int *map_address, memory_region_t regions[])
{
	int i;
	mmap.address = map_address;
	mmap.max_bits = 0xFFFFFFFF / 0x1000;  // Full address space / 4k = total number of pages
	u32int end_address;
	u32int cur_address;

	memset(mmap.address, 0xFF, (mmap.max_bits / 8) + 1); // 8 is the number of blocks per byte

	i = 0;
	while(regions[i].length != 0)
	{
		end_address = (u32int)regions[i].start_addr + regions[i].length - 1;
		kprint("New Region: ");
		khex((u32int)regions[i].start_addr);
		kprint(" - ");
		khex(end_address);
		kprint("\n");

		cur_address = ((u32int)regions[i].start_addr);
		if (cur_address % 0x1000)
			cur_address = (cur_address + 0x1000) & 0xFFFFF000;
		u32int last_index = 0;
		u32int index;
		while(cur_address + 0x1000 < end_address)
		{
			ASSERT(!(cur_address % 0x1000));
			index = get_page_index((u32int*)cur_address);
			if(cur_address != (u32int)regions[i].start_addr)
			{
				ASSERT((index - last_index == 1));
			}
			if (cur_address >= 0x400000)
			{
				clear_bit(index, mmap);
			}
			last_index = index;
			cur_address += 0x1000;
		}
		i++;
	}
	kprint("Available memory initialised\n");
}

u32int *palloc()
{
	u32int index;
	u32int *address;

	index = find_first_clear(mmap);
	if(index == -1)
	{
		PANIC("No free memory left.");
	}
	set_bit(index, mmap);
	address = get_page_address(index);
	return address;
}

u32int pfree(u32int *address)
{
	u32int page_index = get_page_index(address);
	clear_bit(page_index, mmap);
	return 0;
}

u32int get_page_index(u32int *address)
{
	return ((u32int)address & 0xFFFFF000) / 0x1000;
}

u32int *get_page_address(u32int index)
{
	return (u32int*)(index * 0x1000);
}
