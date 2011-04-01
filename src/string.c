#include "string.h"
#include "common.h"

void *memset(void *s, int c, size_t n)
{
	size_t i;
	for(i = 0; i < n; i++)
	{
		*((unsigned char*)s + i) = (unsigned char)c;
	}
}
