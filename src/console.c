#include "console.h"
#include <stdio.h>

void clear_console(void)
{
	printf("\033[2J");
	printf("\033[0;0H");
	fflush(stdout);
	return;
}

void set_color_front(uint8_t R, uint8_t G, uint8_t B)
{
	printf("\033[38;2;%d;%d;%dm", R, G, B);
	fflush(stdout);
	return;
}

void set_color_back(uint8_t R, uint8_t G, uint8_t B)
{
	printf("\033[48;2;%d;%d;%dm", R, G, B);
	fflush(stdout);
	return;
}

void reset_color()
{
	printf("\033[0m");
	fflush(stdout);
	return;
}

void move_up(uint16_t val)
{
	printf("\033[%dA", val);
	fflush(stdout);
	return;
}

void move_down(uint16_t val)
{
	printf("\033[%dB", val);
	fflush(stdout);
	return;
}

void move_right(uint16_t val)
{
	printf("\033[%dC", val);
	fflush(stdout);
	return;
}

void move_left(uint16_t val)
{
	printf("\033[%dD", val);
	fflush(stdout);
	return;
}

void move_to(coord_t dst)
{
	printf("\033[%d;%dH", dst.Y, dst.X);
	fflush(stdout);
	return;
}

void move_to2(uint16_t x, uint16_t y)
{
	printf("\033[%d;%dH", y, x);
	fflush(stdout);
	return;
}

void hide_cursor(void)
{
	printf("\033[?25l");
	fflush(stdout);
	return;
}

void show_cursor(void)
{
	printf("\033[?25h");
	fflush(stdout);
	return;
}

void record_cursor(void)
{
	printf("\033[s");
	fflush(stdout);
	return;
}

void restore_cursor(void)
{
	printf("\033[u");
	fflush(stdout);
	return;
}

