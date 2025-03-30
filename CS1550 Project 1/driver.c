#include "library.c"
#include <time.h>

int main(int argc, char** argv)
{

	init_graphics();
	int i;
	char key;
	clear_screen();

	draw_rect(40, 30, 40, 50, encode_color(30, 0, 0)); // red
	sleep_ms(400);
	draw_rect(90, 80, 60, 15, encode_color(29, 23, 0)); // orange
	sleep_ms(400);
	draw_rect(170, 30, 75, 75, encode_color(31, 30, 23)); // pink
	sleep_ms(400);
	draw_rect(250, 25, 10, 90, encode_color(1, 30, 23)); // blue
	sleep_ms(400);
	draw_rect(320, 25, 50, 30, encode_color(1, 30, 5)); // green
	sleep_ms(400);
	draw_rect(400, 100, 100, 120, encode_color(15, 30, 24)); //lilac
	sleep_ms(400);
	draw_rect(1000, 70, 80, 50, encode_color(31, 11, 16)); 
	//hotpink
	// hotpink draw_rect tests bounds by using x = 1000

	sleep_ms(400);

	// Use key 'p' to write "Hello" to screen, or skip with 'q'
	int iterX = 0;
	int iterY = 0;
	int keyY = 150;
	do{
		key = getkey();
		if(key == 'q' || iterY >= 350 ){
			break;
		}else if(key == 'p'){
			draw_text(150 + iterX, keyY + iterY, "Hello!", 
			encode_color(31, 30, 18));
			iterX += 50;
			iterY += 50;
		}

	}while(1);

	sleep_ms(300);
	draw_circle(500, 350, 10, encode_color(30,0,0)); // red

	draw_rect(40, 350, 50, 100, encode_color(0, 0, 30)); // blue
	sleep_ms(350);
	draw_text(300, 240, "Welcome to SloaneWorld Enterprises!\0",encode_color(30,0,0));
	sleep_ms(400);
	draw_circle(161, 250, 7, encode_color(23,29,21)); 
	sleep_ms(400);
	draw_circle(110, 190, 11, encode_color(30, 60, 30));

	sleep_ms(350);

	for(i = 0; i < 100; i++){
		draw_pixel(200 + i, 250, encode_color(0, 31, 0)); 
		//green line of pixels! :)
	}

	sleep_ms(900);
	clear_screen();

	draw_text(-170, 400, "HOOOOAAAHHHHHHHGGHHHH", encode_color(16,1,10));
	draw_rect(-200, 300, 80, 30, encode_color(16, 13, 14));

	sleep_ms(500);

	draw_rect(300, 300, 70, 40, encode_color(3, 22, 27)); 
	draw_circle(300, 200, 10, encode_color(22,29,22));

	sleep_ms(350);

	int circLoop;
	for(circLoop = 0; circLoop < 10; circLoop++){
		sleep_ms(400);
		draw_circle(40 + (circLoop*30), 50 + (circLoop*10), 5, encode_color(17, 21, 5));
	}

	sleep_ms(300);

	draw_text(320, 240, ",,,,,,,,,,", encode_color(14, 11, 21));

	clear_screen();
	
	sleep_ms(300);
	
	draw_text(200, 240, "SloaneWorld Enterprises Thanks You :)", encode_color(7, 30, 28));

	sleep_ms(1500);

	exit_graphics();
	return 0;

}
