#include "common.h"
#include "pmm.h"
#include "vmm.h"
#include "string.h"
#include "monitor.h"
#include "isr.h"

page_directory_t *kernel_page_dir = (page_directory_t *)0x200000;

void page_fault_handler(registers_t regs)
{
   // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   u32int faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   // The error code gives us details of what happened.
   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   kprint("Page fault! ( ");
   if (present) {kprint("present ");}
   if (rw) {kprint("read-only ");}
   if (us) {kprint("user-mode ");}
   if (reserved) {kprint("reserved ");}
   if (id) {kprint("id");}
   kprint(") at 0x");
   khex(faulting_address);
   kprint("\n");
   PANIC("Page fault");
}

void init_vmm()
{
	int i;

	page_table_t *first_page_table = (page_table_t *) 0x201000;
	memset(first_page_table, 0x00, sizeof(page_table_t));
	memset(kernel_page_dir, 0x00, sizeof(page_directory_t));
	u32int addr;
	u32int cr0In;

	for(i = 0; i < 1024; i++)
	{
		addr = i * 0x1000;
		addr &= PTE_FRAME;
		addr |= (PTE_PRESENT | PTE_WRITEABLE);
		first_page_table->entries[i] = addr;
	}

	addr = (u32int)first_page_table;
	addr &= PDE_FRAME;
	addr |= (PDE_PRESENT | PTE_WRITEABLE);
	kernel_page_dir->entries[0] = addr;

	register_interrupt_handler(14, &page_fault_handler);

	asm volatile ("mov %0, %%cr3"::"r"(kernel_page_dir));
	asm volatile ("mov %%cr0, %0":"=r"(cr0In));
	cr0In |= 0x80000000;
	asm volatile ("mov %0, %%cr0"::"r"(cr0In));
}
