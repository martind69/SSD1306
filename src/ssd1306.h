/*

	Driver for SSD1306 controlled dot matrix OLED/PLED 128x64 displays

	Based on work by Neven Boyanov, Tejashwi Kalp Taru, Bob Tidey

	https://github.com/martind69/SSD1306
	Modified 2024, Martin Dittrich
	All rights reserved.
	
*/

#ifndef SSD1306_H
#define SSD1306_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "mcc_generated_files/i2c1_master.h"
#include "mcc_generated_files/memory.h"
#include "ssd1306fonts.h"

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

void ssd1306_init(uint8_t saddr);
void ssd1306_sleep(bool s);
void ssd1306_setpos(uint8_t x, uint8_t y);
void ssd1306_setscale(uint8_t s);
void ssd1306_fillscreen(unsigned char ch);
void ssd1306_flipscreen(bool f);
void ssd1306_invert(bool i);
void ssd1306_char_move(char ch, uint8_t mode, uint16_t offset);
void ssd1306_char(char ch);
void ssd1306_char_pos(uint8_t x, uint8_t y, char *ch);
void ssd1306_string(char *s);
void ssd1306_string_pos(uint8_t x, uint8_t y, char *s);
void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t bitmap_loc);

uint8_t oledAddr;
uint8_t xpos;
uint8_t ypos;
uint8_t scale;

#endif
