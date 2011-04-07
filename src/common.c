// common.c -- Defines some global functions.
// From JamesM's kernel development tutorials.

#include "common.h"
#include "monitor.h"

// Write a byte out to the specified port.
void outb(u16int port, u8int value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
	u8int ret;
	asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u16int inw(u16int port)
{
	u16int ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void panic(char* message, char* file, u32int line)
{
	asm volatile ("cli");
	kprint("\nPANIC(");
	kprint(message);
	kprint(") at ");
	kprint(file);
	kprint(":");
	kdec(line);
	monitor_put('\n');
	for(;;);
}

void panic_assert(char *file, u32int line, char *desc)
{
	asm volatile("cli");

	kprint("ASSERTION-FAILED(");
	kprint(desc);
	kprint(") at ");
	kprint(file);
	kprint(":");
	kdec(line);
	monitor_put('\n');
	for(;;);
}
