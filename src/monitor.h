// monitor.h -- Defines the interface for monitor.h
// From JamesM's kernel development tutorials.

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

#define CURSOR_LOCATION (cursor_y * max_cols + cursor_x)

// Write a single character out to the screen.
void monitor_put(char c);

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(char *c);
void kprint(char *c);

void khex(u32int num);
void kdec(u32int num);
void monitor_write_base(u32int num, u32int base);
int lzeros(u32int, u32int, u32int);
void padprint(u32int num, u32int base, u32int len, char pad);

u8int *monitor_get_cursor();

void monitor_move_cursor(u8int, u8int);
void move_cursor();

void monitor_scroll();

void monitor_write_to_line(char *c, u8int line);

#endif // MONITOR_H
