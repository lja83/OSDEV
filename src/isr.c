//
// isr.c -- High level interrupt service routines and interrupt request handlers.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "common.h"
#include "isr.h"
#include "monitor.h"
#include "string.h"

isr_t interrupt_handlers[256];

void init_isr_handlers()
{
	memset(&interrupt_handlers, 0, sizeof(isr_t)*256);
}

void register_interrupt_handler(u8int n, isr_t handler)
{
	monitor_write("Registering a handler...\n");
	interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
	monitor_write("received interrupt: ");
	monitor_write_dec(regs.int_no);
	monitor_put('\n');
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
	// Send an EOI (end of interrupt) signal to the PICS.
	// If this interrupt involved the slave
	if (regs.int_no >= 40)
	{
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}

	// Send reset signal to master. (As well as slave, if necessary).
	outb(0x20, 0x20);

	if(interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}
