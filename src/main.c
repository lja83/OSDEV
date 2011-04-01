// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials
#include "common.h"
#include "monitor.h"

int main(struct multiboot *mboot_ptr)
{
	int i;
	// All our initialisation calls will go in here.
	//asm("movl $0xDEADBABA, %eax");
	monitor_clear();
	monitor_write("Hello, World!\nThis is a new line!\n\0");
	monitor_write("Time to test if this stuff worked!\n");
	for(i = (int)'a'; i <= (int)'z'; i++)
	{
		monitor_put((char)i);
		monitor_put(' ');
		monitor_write("\tthis is all you should see\n");
	}

	return 0xDEADBABA;
}
