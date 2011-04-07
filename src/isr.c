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
	interrupt_handlers[n] = handler;
	if (n >= IRQ0)
	{
		monitor_write("IRQ ");
		kdec(n - IRQ0);
	}
	else
	{
		monitor_write("Interrupt ");
		kdec(n);
	}
	monitor_write(" Registered...\n");
}

void unregister_interrupt_handler(u8int n)
{
	monitor_write("Unregistering a handler...\n");
	interrupt_handlers[n] = 0x0;
}

int is_registered(u8int n)
{
	return !(interrupt_handlers[n] == 0);
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
	if(interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
	else
	{
		monitor_write("Unhandled Interrupt: ");
		kdec(regs.int_no);
		monitor_put('\n');
		PANIC("Unhandled Interrupt");
	}
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
	if(interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}

	// Send an EOI (end of interrupt) signal to the PICS.
	// If this interrupt involved the slave
	if (regs.int_no >= 40)
	{
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}

	// Send reset signal to master. (As well as slave, if necessary).
	outb(0x20, 0x20);
}
