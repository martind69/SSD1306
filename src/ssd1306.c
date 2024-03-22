/*

	Driver for SSD1306 controlled dot matrix OLED/PLED 128x64 displays

	Based on work by Neven Boyanov, Tejashwi Kalp Taru, Bob Tidey

	https://github.com/martind69/SSD1306
	Modified 2024, Martin Dittrich
	All rights reserved.
	
*/

#include "ssd1306.h"

static const uint8_t ssd1306_init_sequence[] = {
	0x00,       	// Initialization Sequence
	0xAE,			// Set Display ON/OFF - AE=OFF, AF=ON
	0xD5, 0x80,		// Set display clock divide ratio/oscillator frequency, set divide ratio
	0xA8, 0x3F,		// Set multiplex ratio (1 to 64) ... (height - 1)
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0x40,       	// Set start line address, at 0.
	0x8D, 0x14,		// Charge Pump Setting, 14h = Enable Charge Pump
	0x20, 0x02, 	// Set Memory Addressing Mode - 00h = Horizontal, 01h = Vertical, 10h = Page, 11h = Invalid
	0xA1,       	// Set Segment Re-map
	0xC8,			// Set COM Output Scan Direction
	0xDA, 0x12,		// Set COM Pins Hardware Configuration - 128x32:0x02, 128x64:0x12
	0x81, 0xCF,		// Set contrast control register
	0xD9, 0xF1,		// Set pre-charge period (0x22 or 0xF1)
	0xDB, 0x40,		// Set Vcomh Deselect Level - 0x00: 0.65 x VCC, 0x20: 0.77 x VCC (RESET), 0x30: 0.83 x VCC
	0x2E,			// Deactivate Scroll command
	0xA4,			// Entire Display ON (resume) - output RAM to display
	0xA6,			// Set Normal/Inverse Display mode. A6=Normal; A7=Inverse
	0xAF			// Set Display ON/OFF - AE=OFF, AF=ON
};

static uint8_t i2cBuff[132];

void ssd1306_init(uint8_t saddr) {
	oledAddr = saddr;
	scale = SCALE;
	while(!I2C1_Open(oledAddr)); // sit here until we get the bus..
	I2C1_WriteBlock(oledAddr, ssd1306_init_sequence, sizeof(ssd1306_init_sequence));
	ssd1306_fillscreen(0);
}

void ssd1306_sleep(bool s) {
	i2cBuff[0] = SSD1306_COMMAND;
	if(s) {
		i2cBuff[1] = 0xAE; // display off
		i2cBuff[2] = 0x8D; // disable charge pump
		i2cBuff[3] = 0x10;
	}
	else {
		i2cBuff[1] = 0x8D; // enable charge pump
		i2cBuff[2] = 0x14;
		i2cBuff[3] = 0xAF; // display on
	}
	I2C1_WriteBlock(oledAddr, i2cBuff, 4);
}

void ssd1306_setpos(uint8_t x, uint8_t y) {
	xpos = x;
	ypos = y;
	i2cBuff[0] = SSD1306_COMMAND; 
	i2cBuff[1] = 0xb0 | (y & 0x07);
	i2cBuff[2] = 0x10 | ((x & 0xf0) >> 4); 
	i2cBuff[3] = x & 0x0f; 
	I2C1_WriteBlock(oledAddr, i2cBuff, 4);
}

void ssd1306_setscale(uint8_t s) {
	scale = s;
}

void ssd1306_fillscreen(unsigned char ch) {
	uint8_t i, j;
	for(i = 0; i <= 7; i++) {
		ssd1306_setpos(0, i);
		i2cBuff[0] = SSD1306_DATA;
		for(j = 1; j <= 128; j++) {
			i2cBuff[j] = ch;
		}
		I2C1_WriteBlock(oledAddr, i2cBuff, 129);
	}
}

void ssd1306_flipscreen(bool f) {
	i2cBuff[0] = SSD1306_COMMAND;
	i2cBuff[1] = f ? 0xA0 : 0xA1;
	i2cBuff[2] = f ? 0xC0 : 0xC8;
	I2C1_WriteBlock(oledAddr, i2cBuff, 3);
}

void ssd1306_invert(bool i) {
	i2cBuff[0] = SSD1306_COMMAND;
	i2cBuff[1] = i ? 0xA7 : 0xA6;
	I2C1_WriteBlock(oledAddr, i2cBuff, 2);
}

void ssd1306_char_move(char ch, uint8_t shift, uint16_t offset) {
#if FONT < 2
	uint8_t i, j = 1;
	char c = ch - 32;
	unsigned char f;
	i2cBuff[0] = SSD1306_DATA;
	for(i = 0; i < FONT_WIDTH; i++) {
		f = (unsigned char)(ssd1306xled_font[c * FONT_WIDTH + i + offset]);
		switch(scale) {
			case 0: i2cBuff[j++] = f;
					break;
			case 1: f = (unsigned char)(nibbleDouble[(f >> shift) & 0x0f]);
					i2cBuff[j++] = f;
					i2cBuff[j++] = f;
					break;
			case 2: f = (unsigned char)(diBitQuad[(f >> shift) & 0x03]);
					i2cBuff[j++] = f;
					i2cBuff[j++] = f;
					i2cBuff[j++] = f;
					i2cBuff[j++] = f;
					break;
		}
	}
	I2C1_WriteBlock(oledAddr, i2cBuff, j);
#endif
}

void ssd1306_char(char ch) {
#if FONT == 0
	switch(scale) {
		case 0: ssd1306_char_move(ch, 0, 0);
				break;
		case 1: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, 0);
				ssd1306_setpos(xpos + 12, ypos - 1);
				break;
		case 2: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 2, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 6, 0);
				ssd1306_setpos(xpos + 24, ypos - 3);
				break;
	}
#elif FONT == 1
	switch(scale) {
		case 0: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 0, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos+8, ypos-1);
				break;
		case 1: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 0, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos+16, ypos - 3);
				break;
		case 2: ssd1306_char_move(ch, 0, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 2, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 6, 0);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 0, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 2, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 4, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos, ypos + 1);
				ssd1306_char_move(ch, 6, FONT_TABLEOFFSET);
				ssd1306_setpos(xpos+32, ypos - 7);
				break;
	}
#endif
}

void ssd1306_char_pos(uint8_t x, uint8_t y, char *ch) {
#if FONT < 2
	ssd1306_setpos(x, y);
	ssd1306_char(*ch);
#endif
}

void ssd1306_string(char *s) {
#if FONT < 2
	while(*s) {
		ssd1306_char(*s++);
	}
#endif
}

void ssd1306_string_pos(uint8_t x, uint8_t y, char *s) {
#if FONT < 2
	ssd1306_setpos(x, y);
	ssd1306_string(s);
#endif
}

void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t bitmap_loc) {
	uint16_t j = bitmap_loc;
	uint8_t x, y;
	y0 /= 8;
	y1 /= 8;
	//y1 % 8 ? NULL : y1++;
	for(y = y0; y < y1; y++) {
		ssd1306_setpos(x0, y);
		i2cBuff[0] = SSD1306_DATA;
		for(x = 1; x <= (x1 - x0); x++) {
			i2cBuff[x] = DATAEE_ReadByte(j++);
		}
		// write in blocks of max 128 bytes
		I2C1_WriteBlock(oledAddr, i2cBuff, x);
	}
}
