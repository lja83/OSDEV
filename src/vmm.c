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

}
