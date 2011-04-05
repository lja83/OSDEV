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

void panic(const char* message, const char* file, u32int line)
{
	asm volatile ("cli");
	monitor_write("PANIC(");
	monitor_write(message);
	monitor_write(") at ");
	monitor_write(file);
	monitor_write(":");
	monitor_write_dec(line);
	monitor_put('\n');
	for(;;);
}
