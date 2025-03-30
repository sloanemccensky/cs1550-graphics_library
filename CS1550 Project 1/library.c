#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "iso_font.h"
#include "graphics.h"
#include <sys/select.h>
#include <sys/types.h>
#include <termios.h>
#include <linux/fb.h>
#include <fcntl.h>


struct fb_var_screeninfo v_screen_info;

struct fb_fix_screeninfo f_screen_info;

struct termios termiosOG;

struct termios termiosMod;

fd_set fd2;
int mmap_fd = 0;
void* framebuffer;
int max_pixels;

void init_graphics(){

	// initializes channel between me and FB
	// ioctl is a syscall for I/O, custom command use
	// v and f screen exist outside of ioctl
	int fd = open("/dev/fb0", O_RDWR);
	ioctl(fd, FBIOGET_VSCREENINFO, &v_screen_info);
	ioctl(fd, FBIOGET_FSCREENINFO, &f_screen_info);

	// Me and something else are accessing a memory array -> mmap
	// mmap est. a FB (link between me and GPU)
	// yres = number of rows // line length is length of the rows 
	// (in bytes)
	framebuffer = mmap(0, (v_screen_info.yres_virtual * f_screen_info.line_length), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	max_pixels = v_screen_info.yres_virtual * f_screen_info.line_length;

	ioctl(0, TCGETS, &termiosOG);
	ioctl(0, TCGETS, &termiosMod);
	termiosMod.c_lflag &= ~ICANON;
        termiosMod.c_lflag &= ~ECHO;
	ioctl(0, TCSETS, &termiosMod);
	//printf("yres %d", v_screen_info.yres_virtual);
	//printf("line length %d", f_screen_info.line_length);
	close(fd);
}

void exit_graphics(){

        // Cleans up memory, undoes key disabling, then clears screen
        munmap(framebuffer, (v_screen_info.yres_virtual * f_screen_info.line_length));
       // termiosOG.c_lflag |= ICANON;
       // termiosOG.c_lflag |= ECHO;
        ioctl(0, TCSETS, &termiosOG);
	clear_screen();

}

char getkey(){

	// Takes in a user-inputted key and returns it
        // select opens the channel for monitoring

        struct timeval gk_time;
        gk_time.tv_usec = 0;
        gk_time.tv_sec = 0;

        FD_ZERO(&fd2);
        FD_SET(0, &fd2);

	// select monitors over a range from 0 to the first arg (just 0) 
        int select_val = select(1, &fd2, NULL, NULL, &gk_time);
        if( select_val < 0 ){
                return NULL;
        }

        char ret = NULL;
        read(0, &ret, sizeof(char));
        return ret;

}

void sleep_ms(long ms){

	// Makes the program "sleep" between frames

	// Sleep time can't be negative
	if(ms < 0){
		return;
	}

	long ns_total = ms * 1000000;

	// timespec_var.tv_nsec = (ms % 1000) * 1000000;
	struct timespec timespec_var;
	timespec_var.tv_sec = ms / 1000;
	timespec_var.tv_nsec = ns_total % 1000000000; 
	nanosleep(&timespec_var, NULL);

}

void clear_screen(){

	// Clears the screen using an ANSI escape code
	
	write(STDOUT_FILENO, "\033[2J", 5);

}

void draw_pixel(int x, int y, color_t color){
	
	// Draws colored pixels to the screen

	// color depth is 2 bytes per pixel
	// lowest pixel is 0,0 // max is max_pixels
	// lowest is base framebuffer

	// FB is an array of bytes (chars), task is to find location
	// But once it's casted, it's in colors, which are two bytes
	// reallinelength counts by two bytes
	
	int real_line_length = f_screen_info.line_length / sizeof(color_t);
	color_t* location = (color_t*) framebuffer + ((y*real_line_length) + x);
	if ( (location >= (color_t*) framebuffer) && (location <= (color_t*) framebuffer + max_pixels) ){
		*(location) = color;
        }
       // printf("Y: %d/tX: %d/n", x, y);
}

void draw_rect(int x1, int y1, int width, int height, color_t c){

	// Draws a rectangle to the screen

	int x;
	int y;

	for(x = x1; x < x1 + width; x++){
		for(y = y1; y < y1 + height; y++){
			draw_pixel(x, y, c);
		}
	}
}

void draw_text(int x, int y, const char *text, color_t c){

	// Draws text to the screen
	// Each letter is every 16 bytes

	int i;
	int j;

	for(;*(text);text++){ // Pointer arithmetic to determine when an empty slot is found
		char *iso_pos = iso_font + ((unsigned int)*(text) * 16);
	//	printf("Index: %d\n", (int) *text);	
	//	printf("0x%02x text value \n", (int) *text);
		for(i = 0; i < 16; i++){
			char row = *(iso_pos + i);
	//		printf("0x%02x row value \n", row);
			for(j = 0; j < 8; j++){
				if( row & (1 <<j) ){
	//				printf("pixel is valid");
					draw_pixel(x+j, y+i, c);
				}
			}
		}
		x += 8;
	}
}

void draw_circle(int xOS, int yOS, int r, color_t c){

	// Draws a circle to the screen
	// Utilizes Jesko's Method for implementation

	int x = r; // radius away on x axis
	int y = 0; 
	float t1 = r / 16;
	float t2;

	while( x >= y ){ //inbetween pixels, choosing based on pixelmidpoint

		draw_pixel(x + xOS, y + yOS, c);
		draw_pixel(y + xOS, x + yOS, c);
		draw_pixel(-x + xOS, y + yOS, c);
		draw_pixel(x + xOS, -y + yOS, c);
		draw_pixel(-x + xOS, -y + yOS, c);
		draw_pixel(-y + xOS, -x + yOS, c);
		draw_pixel(-y + xOS, x + yOS, c);
		draw_pixel(y + xOS, -x + yOS, c);

		y++;
		t1 += y;
		t2 = t1 - x;
		if( t2 >= 0 ){
			t1 = t2;
			x--;
		}
	}
}


