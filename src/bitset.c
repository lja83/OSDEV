#include "bitset.h"
#include "common.h"

#define FIELD_WIDTH 32
#define FULL_FIELD 0xFFFFFFFF

void set_bit(int offset, bitset_t bitset)
{
	*(bitset.address + (offset / FIELD_WIDTH)) |= (1 << (offset % FIELD_WIDTH));
}

void clear_bit(int offset, bitset_t bitset)
{
	*(bitset.address + (offset / FIELD_WIDTH)) &= ~(1 << (offset % FIELD_WIDTH));
}

int test_bit(int offset, bitset_t bitset)
{
	return *(bitset.address + (offset / FIELD_WIDTH)) & (1 << (offset % FIELD_WIDTH));
}

int find_first_clear(bitset_t bitset)
{
	int i, j;
	u32int *cur_address;
	for(i = 0; i < bitset.max_bits; i += FIELD_WIDTH)
	{
		cur_address = *(bitset.address + (i / FIELD_WIDTH));
		if(*cur_address != FULL_FIELD)
		{
			for(j = 0; j < FIELD_WIDTH; j++)
			{
				if(test_bit(i+j, bitset) == 0)
				{
					return i+j;
				}
			}
		}
	}
	return -1;
}
