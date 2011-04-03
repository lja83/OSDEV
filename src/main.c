// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"

int main(struct multiboot *mboot_ptr)
{
	int i;
	// All our initialisation calls will go in here.
	//asm("movl $0xDEADBABA, %eax");
	monitor_clear();
	init_descriptor_tables();
	init_isr_handlers();

	asm volatile("int $0x3");
	asm volatile("int $0x4");

	monitor_write("Returning from main.  Goodbye!\n");
	return 0xDEADBABA;
}
