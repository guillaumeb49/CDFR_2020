// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).
#include "F_LCD.h"

#include <stdio.h>
#include "math.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "F_Odometry.h"

void F_LCD_Init(void){
	g_addr = LCD_ADDR;
	g_cols = LCD_COLS;
	g_rows = LCD_ROWS;
	g_charsize = LCD_CHARSIZE;
	g_backlightval = LCD_BACKLIGHT;

	LL_I2C_Enable(I2C1);

	F_LCD_begin();
}
void F_LCD_begin() {
	g_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (g_rows > 1) {
		g_displayfunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((g_charsize != 0) && (g_rows == 1)) {
		g_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	HAL_Delay(50);

	// Now we pull both RS and R/W low to begin commands
	D_LCD_expanderWrite(g_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	HAL_Delay(100);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	D_LCD_write4bits(0x03 << 4);
	HAL_Delay(5); // wait min 4.1ms

	// second try
	D_LCD_write4bits(0x03 << 4);
	HAL_Delay(5); // wait min 4.1ms

	// third go!
	D_LCD_write4bits(0x03 << 4);
	HAL_Delay(5);

	// finally, set to 4-bit interface
	D_LCD_write4bits(0x02 << 4);

	// set # lines, font size, etc.
	F_LCD_command(LCD_FUNCTIONSET | g_displayfunction);

	// turn the display on with no cursor or blinking default
	g_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	F_LCD_display();

	// clear it off
	F_LCD_clear();

	// Initialize to default text direction (for roman languages)
	g_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	F_LCD_command(LCD_ENTRYMODESET | g_displaymode);

	F_LCD_home();
}
/********** high level commands, for the user! */
void F_LCD_clear(){
	F_LCD_command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	HAL_Delay(50);  // this command takes a long time!
}
void F_LCD_home(){
	F_LCD_command(LCD_RETURNHOME);  // set cursor position to zero
	HAL_Delay(50);  // this command takes a long time!
}
void F_LCD_setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > g_rows) {
		row = g_rows-1;    // we count rows starting w/0
	}
	F_LCD_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}
// Turn the display on/off (quickly)
void F_LCD_noDisplay() {
	g_displaycontrol &= ~LCD_DISPLAYON;
	F_LCD_command(LCD_DISPLAYCONTROL | g_displaycontrol);
}
void F_LCD_display() {
	g_displaycontrol |= LCD_DISPLAYON;
	F_LCD_command(LCD_DISPLAYCONTROL | g_displaycontrol);
}
// Turns the underline cursor on/off
void F_LCD_noCursor() {
	g_displaycontrol &= ~LCD_CURSORON;
	F_LCD_command(LCD_DISPLAYCONTROL | g_displaycontrol);
}
void F_LCD_cursor() {
	g_displaycontrol |= LCD_CURSORON;
	F_LCD_command(LCD_DISPLAYCONTROL | g_displaycontrol);
}
// Turn on and off the blinking cursor
void F_LCD_noBlink() {
	g_displaycontrol &= ~LCD_BLINKON;
	F_LCD_command(LCD_DISPLAYCONTROL | g_displaycontrol);
}
void F_LCD_blink() {
	g_displaycontrol |= LCD_BLINKON;
	F_LCD_command(LCD_DISPLAYCONTROL | g_displaycontrol);
}
// These commands scroll the display without changing the RAM
void F_LCD_scrollDisplayLeft(void) {
	F_LCD_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void F_LCD_scrollDisplayRight(void) {
	F_LCD_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}
// This is for text that flows Left to Right
void F_LCD_leftToRight(void) {
	g_displaymode |= LCD_ENTRYLEFT;
	F_LCD_command(LCD_ENTRYMODESET | g_displaymode);
}
// This is for text that flows Right to Left
void F_LCD_rightToLeft(void) {
	g_displaymode &= ~LCD_ENTRYLEFT;
	F_LCD_command(LCD_ENTRYMODESET | g_displaymode);
}
// This will 'right justify' text from the cursor
void F_LCD_autoscroll(void) {
	g_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	F_LCD_command(LCD_ENTRYMODESET | g_displaymode);
}
// This will 'left justify' text from the cursor
void F_LCD_noAutoscroll(void) {
	g_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	F_LCD_command(LCD_ENTRYMODESET | g_displaymode);
}
// Allows us to fill the first 8 CGRAM locations
// with custom characters
void F_LCD_createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	F_LCD_command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		F_LCD_write(charmap[i]);
	}
}
// Turn the (optional) backlight off/on
void F_LCD_noBacklight(void) {
	g_backlightval=LCD_NOBACKLIGHT;
	D_LCD_expanderWrite(0);
}
void F_LCD_backlight(void) {
	g_backlightval=LCD_BACKLIGHT;
	D_LCD_expanderWrite(0);
}
uint8_t F_LCD_getBacklight() {
  return g_backlightval == LCD_BACKLIGHT;
}

/*********** mid level commands, for sending data/cmds */

void F_LCD_command(uint8_t value) {
	D_LCD_send(value, 0);
}
void F_LCD_write(uint8_t value) {
	D_LCD_send(value, LCD_RS);
}
void F_LCD_printstr(char *c){
	//This function is not identical to the function used for "real" I2C displays
	//it's here so the user sketch doesn't have to be changed
	int idx=0;
	while (c[idx] !='\0' && idx<16) {
		F_LCD_write(c[idx]);
		idx++;
	}
}
//void F_LCD_PrintPosition(Localisation Loc){
//	char line1[16];
//	char line2[16];
//
//	//(teta_deg<0) ? (signe = '-') : (signe = '+');
//
//	sprintf(line1,"X:%4d Y:%4d",(int)Loc.x,(int)Loc.y);
//	sprintf(line2,"Angle : %4d",(int)F_Math_toDeg(Loc.teta));
//
//	F_LCD_setCursor(0,0);
//	F_LCD_printstr(line1);
//	F_LCD_setCursor(0,1);
//	F_LCD_printstr(line2);
//
//}
//void F_LCD_PrintAngles(Localisation Loc){
//	char line1[16];
//	char line2[16];
//
//
//	sprintf(line1,"P:%4d R:%4d",(int)Loc.pitch,(int)Loc.roll);
//	sprintf(line2,"Yaw:%4d",(int)Loc.yaw);
//
//	F_LCD_setCursor(0,0);
//	F_LCD_printstr(line1);
//	F_LCD_setCursor(0,1);
//	F_LCD_printstr(line2);
//
//}
/************ low level data pushing commands **********/

// write either command or data
void D_LCD_send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;

	D_LCD_write4bits((highnib)|mode);
	D_LCD_write4bits((lownib)|mode);
}
void D_LCD_write4bits(uint8_t value) {
	D_LCD_expanderWrite(value);
	D_LCD_pulseEnable(value);
}
void D_LCD_expanderWrite(uint8_t a_data){
	F_I2C1_WriteValue(g_addr,a_data | g_backlightval);
}
void D_LCD_pulseEnable(uint8_t _data){
	D_LCD_expanderWrite(_data | LCD_EN);	// En high
	HAL_Delay(1);		// enable pulse must be >450ns

	D_LCD_expanderWrite(_data & ~LCD_EN);	// En low
	//HAL_Delay(1);		// commands need > 37us to settle
}
void D_LCD_load_custom_character(uint8_t char_num, uint8_t *rows){
	F_LCD_createChar(char_num, rows);
}
void D_LCD_setBacklight(uint8_t new_val){
	if (new_val) {
		F_LCD_backlight();		// turn backlight on
	} else {
		F_LCD_noBacklight();		// turn backlight off
	}
}

void F_LCD_DebugTask_Handler(void const * argument){

	// 1. Initialization message
	char line1[16],line2[16];
	sprintf(line1,"Time %s",__TIME__);
	sprintf(line2,"Date%s",__DATE__);

	F_LCD_Init();
	F_LCD_setCursor(0,0);
	F_LCD_printstr(line1);
	F_LCD_setCursor(0,1);
	F_LCD_printstr(line2);

	// 2. Wait 1s and clear Initialization message
	osDelay(1000);
	F_LCD_clear();

    while(1){
        // 3. Wait until period elapse
    	osDelay(300);
    	F_GPIO_SetLed4(TRUE);	// Flag ON

    	// 4. Fit data for printing
    	F_Odometry_printCountersLCD();

    	// 6. Set flag to LOW.
    	F_GPIO_SetLed4(FALSE);	// Flag OFF
    }
}
