#ifndef BITSET_H_
#define BITSET_H_

#include "common.h"

typedef struct {
	u32int max_bits;
	u32int *address;
} bitset_t;

void set_bit(int offset, bitset_t bitset);
void clear_bit(int offset, bitset_t bitset);
int test_bit(int offset, bitset_t bitset);
int find_first_clear(bitset_t bitset);

#endif /* BITSET_H_ */
