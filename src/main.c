// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"
#include "keyboard.h"
#include "kheap.h"

void heapTest()
{
	monitor_write("Heap test...\n");
	u32int a = kmalloc(8);
	u32int b = kmalloc(8);
	monitor_write("a: ");
	monitor_write_hex(a);
	monitor_write("\nb: ");
	monitor_write_hex(b);

	kfree(b);
	kfree(a);

	u32int c = kmalloc(16);
	monitor_write("\nc: ");
	monitor_write_hex(c);
	monitor_put('\n');
}

int main(struct multiboot *mboot_ptr)
{
	monitor_clear();
	monitor_put('\n');			// Reserve the top row for the clock ticks

	init_descriptor_tables();
	init_timer(50);
	initialise_paging();
	register_interrupt_handler(IRQ1, &keyboard);
	asm volatile("sti");

	monitor_write("Press T to toggle registration of the timer interrupt handler.\n");
	monitor_write("Press U to generate an unhandled interrupt.\n");
	monitor_write("Press P to generate a page fault.\n");
	monitor_write("Press A to cause an assertion failure\n");
	monitor_write("Press H to test the heap.\n");

	return 0;
}
