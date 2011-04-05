// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"
#include "keyboard.h"

int main(struct multiboot *mboot_ptr)
{
	monitor_clear();
	monitor_put('\n');			// Reserve the top row for the clock ticks

	init_descriptor_tables();
	init_timer(50);

	register_interrupt_handler(IRQ1, &keyboard);
	initialise_paging();
	asm volatile("sti");

	monitor_write("Press T to toggle registration of the timer interrupt handler.\n");
	monitor_write("Press U to generate an unhandled interrupt.\n");
	monitor_write("Press P to generate a page fault.\n");

	return 0;
}
