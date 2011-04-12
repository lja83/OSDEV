#ifndef VMM_H_
#define VMM_H_

#include "common.h"

#define PDE_PRESENT			(1<<0)
#define PDE_WRITEABLE		(1<<1)
#define PDE_USER			(1<<2)
#define PDE_WRITETHROUGH	(1<<3)
#define PDE_DISABLE_CACHE	(1<<4)
#define PDE_ACCESSED		(1<<5)
#define PDE_DIRTY			(1<<6)
#define PDE_HUGE_PAGES		(1<<7)
#define PDE_GLOBAL			(1<<8)
#define PDE_AVAIL			(7<<9)
#define PDE_FRAME			(0x7FFFF<<12)

#define PTE_PRESENT			(1<<0)
#define PTE_WRITEABLE		(1<<1)
#define PTE_USER			(1<<2)
#define PTE_WRITETHROUGH	(1<<3)
#define PTE_DISABLE_CACHE	(1<<4)
#define PTE_ACCESSED		(1<<5)
#define PTE_DIRTY			(1<<6)
#define PTE_HUGE_PAGES		(1<<7)
#define PTE_GLOBAL			(1<<8)
#define AVAIL				(7<<9)
#define PDE_FRAME			(0x7FFFF<<12)

typedef u32int page_table_entry_t;
typedef u32int page_directory_entry_t;



void init_vmm();

#endif /* VMM_H_ */
