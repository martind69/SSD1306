/*
 * SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64 displays
 *
 * @created: 2020-10-05
 * @author: Bob Tidey
 *
 * Source code available at: https://github.com/roberttidey/SSD1306BB
 * 
 * Based on work by Neven Boyanov, Tejashwi Kalp Taru
 * 
 * Modified by Martin Dittrich
*/

#include <xc.h>
#include <stdint.h>
#include "ssd1306fonts.h"
#include "mcc_generated_files/examples/i2c_master_example.h"

#ifndef SSD1306_H
#define SSD1306_H

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

void ssd1306_init(uint8_t saddr);
void ssd1306_sleep(uint8_t s);
void ssd1306_setpos(uint8_t x, uint8_t y);
void ssd1306_setscale(uint8_t s);
void ssd1306_fillscreen(uint8_t fill);
void ssd1306_flipscreen(uint8_t flip);
void ssd1306_char_move(char ch, uint8_t mode, uint16_t offset);
void ssd1306_char(char ch);
void ssd1306_string(char *s);
void ssd1306_string_pos(uint8_t x, uint8_t y, char *s);
void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]);

uint8_t oledAddr;
uint8_t xpos;
uint8_t ypos;
uint8_t scale;

#endif
