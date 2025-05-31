#include "sys/video.h"

#define OS_NAME "IRIS-OS"
#define OS_VERSION "0.1.3"

void klog(const char* msg, char status_color, const char* status) {
	set_color(0x07);
	printf("[  ");
	set_color(status_color);
	printf(status);
	set_color(0x07);
	printf("  ] ");
	printf(msg);
	new_line();
}

void print_welcome() {
	if(video_init())
		klog("Video subsystem initialized.", 0x02, "OK");
	else
		klog("Initializing video subsystem...", 0x0c, "FAIL");
	new_line();
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
