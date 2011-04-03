//
// isr.c -- High level interrupt service routines and interrupt request handlers.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "common.h"
#include "isr.h"
#include "monitor.h"
#include "string.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
	monitor_write("received interrupt: ");
	monitor_write_hex(regs.int_no);
	monitor_put('\n');
}
