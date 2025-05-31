#include "sys/video.h"
#include "sys/io.h"
#include <stdarg.h>

#define VWIDTH 80
#define VHEIGHT 25
#define VMEM_PTR (char*) 0xb8000

static int cursor_x = 0;
static int cursor_y = 0;
static char current_color = 0x07;

char video_init() {
    clear_screen();
    set_color(0x07);
    set_cursor(0, 0);

    char* video = VMEM_PTR;
    char backup = video[0];
    video[0] = 'x';
    if(video[0] != 'x') return VFALSE;
    video[0] = backup;
    return VTRUE;
}

void clear_screen() {
    char* video = VMEM_PTR;
    for(int i = 0; i < VWIDTH * VHEIGHT; ++i) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = current_color;
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void set_cursor(int x, int y) {
    cursor_x = x;
    cursor_y = y;
    move_cursor();
}

void move_cursor() {
    unsigned short pos = cursor_y * VWIDTH + cursor_x;
    
    outb(0x3d4, 14);
    outb(0x3d5, (pos >> 8) & 0xff);
    outb(0x3d4, 15);
    outb(0x3d5, pos & 0xff);
}

void putchar(char c) {
    char* video = VMEM_PTR;

    if(c == '\n') {
        new_line();
        return;
    }

    int offset = (cursor_y * VWIDTH + cursor_x) * 2;
    video[offset] = c;
    video[offset + 1] = current_color;

    ++cursor_x;
    if(cursor_x >= VWIDTH) new_line();

    move_cursor();
}

void new_line() {
    cursor_x = 0;
    ++cursor_y;
    if(cursor_y >= VWIDTH) {
        char* video = VMEM_PTR;
        for(int y = 1; y < VHEIGHT; ++y) {
            for(int x = 0; x < VWIDTH; ++x) {
                int dst = (y - 1) * VWIDTH + x;
                int src = y * VWIDTH + x;

                video[dst * 2] = video[src * 2];
                video[dst * 2 + 1] = video[src * 2 + 1];
            }
        }

        for(int x = 0; x < VWIDTH; ++x) {
            int i = (VHEIGHT - 1) * VWIDTH + x;
            video[i * 2] = ' ';
            video[i * 2 + 1] = current_color;
        }

        cursor_y = VHEIGHT - 1;
    }
}

void print(const char* str) {
    while(*str) {
        putchar(*str++);
    }
}

void print_color(const char* str, char color) {
    char old_color = current_color;
    current_color = color;
    print(str);
    current_color = old_color;
}

static void print_dec(int value) {
    char buffer[10];
    int i = 0;
    if (value == 0) {
        putchar('0');
        return;
    }
    if(value < 0 ) {
        putchar('-');
        value = -value;
    }
    while(value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    while(i--) {
        putchar(buffer[i]);
    }
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for(const char* p = fmt; *p != '\0'; ++p) {
        if(*p == '%') {
            ++p;
            if(*p == 'd') {
                int i = va_arg(args, int);
                print_dec(i);
            } else if (*p == 's') {
                char* s = va_arg(args, char*);
                for(; *s; ++s) putchar(*s);
            } else if (*p == '%')
                putchar('%');
            else
                putchar(*p);
            
        } else putchar(*p);
    }

    va_end(args);
}

void set_color(char color) {
    current_color = color;
}