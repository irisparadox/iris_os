#ifndef OSI_VIDEO_H
#define OSI_VIDEO_H

#include <stdint.h>

#define VTRUE  1
#define VFALSE 0

char video_init();
void clear_screen();
void set_color(char color);
void putchar(char c);
void print(const char* str);
void print_color(const char* str, char color);
void printf(const char* fmt, ...);
void set_cursor(int x, int y);
void move_cursor();
void new_line();

#endif //OSI_VIDEO_H