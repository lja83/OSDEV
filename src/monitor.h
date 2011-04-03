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

void monitor_write_dec(u32int num);

void monitor_write_hex(u32int num);
void write_hex(u32int num);

void move_cursor();

void monitor_scroll();

void monitor_write_to_line(char *c, u8int line);

#endif // MONITOR_H
