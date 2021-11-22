#ifndef CONSOLE_H
#define CONSOLE_H 1

#include <stdint.h>

#if defined(__cplusplus)
   extern "C" {
#endif

typedef struct _coord{
	uint16_t X;
	uint16_t Y;
}coord_t;

typedef int file_t;

void clear_console(void);

void set_color_front(uint8_t R, uint8_t G, uint8_t B);

void set_color_back(uint8_t R, uint8_t G, uint8_t B);

void reset_color(void);

void move_up(uint16_t val);

void move_down(uint16_t val);

void move_right(uint16_t val);

void move_left(uint16_t val);

void move_to(coord_t dst);

void move_to2(uint16_t x, uint16_t y);

void hide_cursor(void);

void show_cursor(void);

void record_cursor(void);

void restore_cursor(void);

#if defined(__cplusplus)
   }
#endif

#endif

