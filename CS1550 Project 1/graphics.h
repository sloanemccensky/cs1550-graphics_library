#include <time.h>

typedef unsigned short color_t;

#define encode_color(r, g, b) (r << 11) | (g << 5) | (b)

void init_graphics();

void exit_graphics();

char getkey();

void sleep_ms(long ms);

void clear_screen();

void draw_pixel(int x, int y, color_t color);

void draw_rect(int x1, int y1, int width, int height, color_t c);

void draw_text(int x, int y, const char *text, color_t c);

void draw_circle(int x, int y, int r, color_t c);
