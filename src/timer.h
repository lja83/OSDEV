// timer.h -- Defines the interface for all PIT-related functions.
// Written for JamesM's kernel development tutorials.

#ifndef TIMER_H
#define TIMER_H

#include "isr.h"
#include "common.h"

void init_timer(u32int frequency);
void timer_callback(registers_t regs);

#endif // TIMER_H
