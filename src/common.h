// common.h -- Defines typedefs and some global functions.
// From JamesM's kernel development tutorials

#ifndef COMMON_H
#define COMMON_H

#include "multiboot.h"

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs ar written for 32-bit X86.
typedef unsigned int 	u32int;
typedef          int	s32int;
typedef unsigned short	u16int;
typedef          short  s16int;
typedef unsigned char	u8int;
typedef          char	s8int;
typedef unsigned int	size_t;

#define NULL 0x0

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0:panic_assert(__FILE__, __LINE__, #b));

void panic(char* msg, char *file, u32int line);
void panic_assert(char *file, u32int line, char *desc);

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

multiboot_info_t *mboot_ptr;
void meminfo();

#endif // COMMON_H
