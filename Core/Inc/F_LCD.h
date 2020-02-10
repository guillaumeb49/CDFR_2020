/*
 * D_LCD.h
 *
 *  Created on: Aug 4, 2018
 *      Author: mario
 */

#ifndef F_LCD_H_
#define F_LCD_H_

#include "i2c.h"

///* Librairie Arduino */
//

#define I2C_LCD_ADD 0x27

////
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_CHARSIZE 4

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define LCD_EN 0b00000100  // Enable bit
#define LCD_RW 0b00000010  // Read/Write bit
#define LCD_RS 0b00000001  // Register select bit

uint8_t g_addr;
uint8_t g_displayfunction;
uint8_t g_displaycontrol;
uint8_t g_displaymode;
uint8_t g_cols;
uint8_t g_rows;
uint8_t g_charsize;
uint8_t g_backlightval;


void F_LCD_Init(void);
void F_LCD_begin();
void F_LCD_clear();
void F_LCD_home();
void F_LCD_noDisplay();
void F_LCD_display();
void F_LCD_noBlink();
void F_LCD_blink();
void F_LCD_noCursor();
void F_LCD_cursor();
void F_LCD_scrollDisplayLeft();
void F_LCD_scrollDisplayRight();
void F_LCD_printLeft();
void F_LCD_printRight();
void F_LCD_leftToRight();
void F_LCD_rightToLeft();
void F_LCD_shiftIncrement();
void F_LCD_shiftDecrement();
void F_LCD_noBacklight();
void F_LCD_backlight();
uint8_t F_LCD_getBacklight();
void F_LCD_autoscroll();
void F_LCD_noAutoscroll();
void F_LCD_createChar(uint8_t, uint8_t[]);
void F_LCD_setCursor(uint8_t, uint8_t);
void F_LCD_write(uint8_t);
void F_LCD_command(uint8_t);
void F_LCD_printstr(char*);
//	void F_LCD_PrintPosition(Localisation);
void F_LCD_PrintSpeed(int,int);
void F_LCD_PrintQEI(void);
//	void F_LCD_PrintAngles(Localisation);

void D_LCD_setBacklight(uint8_t new_val);				// alias for backlight() and nobacklight()
void D_LCD_load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()
void D_LCD_send(uint8_t, uint8_t);
void D_LCD_write4bits(uint8_t);
void D_LCD_expanderWrite(uint8_t);
void D_LCD_pulseEnable(uint8_t);

void F_LCD_DebugTask_Handler(void const * argument);
#endif /* F_LCD_H_ */
