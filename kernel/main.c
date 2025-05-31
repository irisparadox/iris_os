#include "sys/video.h"

#define OS_NAME "IRIS-OS"
#define OS_VERSION "0.1.2"

void print_welcome() {
	video_init();
	clear_screen();
	printf("Welcome to ");
	set_color(0x9f);
	printf(OS_NAME);
	new_line();
	set_color(0x07);
	printf("Version ");
	set_color(0x0a);
	printf(OS_VERSION);
	new_line();
	move_cursor();
}

void kernel_main() {
	print_welcome();

	while(1);
}
