/**
 * @file ws0010.c
 * @author Thanasis Georgiou
 * @brief Implements all functions for the WS0010 component.
 *
 * This component is designed to be mostly compatible with the stock Character LCD component. There are
 * some quirks here and there but transition should be painless.
 */

#include <project.h>
#include "cytypes.h"
#include "stdlib.h"
#include "cyfitter.h"

#include "`$INSTANCE_NAME`_ws0010.h"
#include "`$INSTANCE_NAME`_util.h"

// Offset for each row in DDRAM
unsigned char row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

void `$INSTANCE_NAME`_Start(void) {
    `$INSTANCE_NAME`_Init();
}

void `$INSTANCE_NAME`_Init(void) {
    // Wait for power-on
    CyDelay(75);
    
    // Put in 8-bit mode
    `$INSTANCE_NAME`_SendNibble(0x03);
    CyDelayUs(5000);
    `$INSTANCE_NAME`_SendNibble(0x08);
    CyDelayUs(5000);
    
    // Put back in 4-bit mode    
    `$INSTANCE_NAME`_SendNibble(0x02);
    CyDelayUs(5000);
    `$INSTANCE_NAME`_SendNibble(0x02);
    CyDelayUs(5000);
    `$INSTANCE_NAME`_SendNibble(0x08);
    CyDelayUs(5000);
  
    // Turn display off
    `$INSTANCE_NAME`_WriteControl(LCD_DISPLAY_CONTROL | LCD_DISPLAY_OFF);
    CyDelayUs(500);
    
    // Clear Display
    `$INSTANCE_NAME`_WriteControl(LCD_CLEAR_DISPLAY);
    CyDelayUs(500);
    
    // Set Entry Mode - Left
    `$INSTANCE_NAME`_WriteControl(LCD_ENTRY_MODE_SET | LCD_ENTRY_LEFT);
    CyDelayUs(500);
    
    // Home Cursor
    `$INSTANCE_NAME`_WriteControl(LCD_RETURN_HOME);
    CyDelayUs(500);
    
    // Turn On
    `$INSTANCE_NAME`_Wakeup();
}

void `$INSTANCE_NAME`_WriteControl(unsigned char cmd) {
    // Control + Write mode
    `$INSTANCE_NAME`_ControlMode();
    `$INSTANCE_NAME`_WriteMode();

    // Send bytes
    `$INSTANCE_NAME`_SendByte(cmd);

    // Wait for the display to stop working
    `$INSTANCE_NAME`_IsReady();
}

void `$INSTANCE_NAME`_WriteData(unsigned char data) {
    // Data + Write mode
    `$INSTANCE_NAME`_DataMode();
    `$INSTANCE_NAME`_WriteMode();
    
    // Send bytes
    `$INSTANCE_NAME`_SendByte(data);

    // Wait for the display to stop working
    `$INSTANCE_NAME`_IsReady();
}

void `$INSTANCE_NAME`_SendByte(unsigned char byte) {
    // Send high
    `$INSTANCE_NAME`_SendNibble(byte >> 4);
    
    // Send low
    `$INSTANCE_NAME`_SendNibble(byte);
}

void `$INSTANCE_NAME`_SendNibble(unsigned char nibble) {
    // Send high
    `$INSTANCE_NAME`_Pins_Data_Write(nibble);
    CyDelayUs(50);
    `$INSTANCE_NAME`_Pin_Enable_Write(HIGH);
    CyDelayUs(50);
    `$INSTANCE_NAME`_Pin_Enable_Write(LOW);
    CyDelayUs(50);
}

void `$INSTANCE_NAME`_IsReady(void) {
    unsigned char isBusy = 1;
    
    `$INSTANCE_NAME`_ControlMode(); // Control mode
    `$INSTANCE_NAME`_ReadMode(); // Read mode
    `$INSTANCE_NAME`_Pins_Data_Write(0x8);

    do {
        // Get the four first bits
        `$INSTANCE_NAME`_Pin_Enable_Write(LOW);
        CyDelayUs(10);
        `$INSTANCE_NAME`_Pin_Enable_Write(HIGH);
        
        CyDelayUs(10);
        isBusy = (`$INSTANCE_NAME`_Pins_Data_Read() >> 3);
        `$INSTANCE_NAME`_Pin_Enable_Write(LOW);
        
        // Get the rest 4 bits, not used
        CyDelayUs(50);
        `$INSTANCE_NAME`_Pin_Enable_Write(HIGH);
        CyDelayUs(50);
        `$INSTANCE_NAME`_Pin_Enable_Write(LOW);
    } while (isBusy == 1);
    
    // Set to write mode
    `$INSTANCE_NAME`_WriteMode();
}

void `$INSTANCE_NAME`_Position(uint8_t column, uint8_t row) {
  if ( row >= 2 ) {
    row = 0;  //write to first line if out off bounds
  }
  
  `$INSTANCE_NAME`_WriteControl(LCD_SET_DDRAM_ADDR | (column + row_offsets[row]));
}

void `$INSTANCE_NAME`_Sleep() {
    `$INSTANCE_NAME`_WriteControl(LCD_DISPLAY_CONTROL | LCD_DISPLAY_OFF);
    `$INSTANCE_NAME`_WriteControl(LCD_CURSOR_SHIFT | LCD_POWER_OFF);
}

void `$INSTANCE_NAME`_Wakeup() {
    `$INSTANCE_NAME`_WriteControl(LCD_CURSOR_SHIFT | LCD_POWER_ON | LCD_CHARACTER_MODE);
    `$INSTANCE_NAME`_WriteControl(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
}

void `$INSTANCE_NAME`_PrintString(char *string) {
    unsigned char index = 0;
    while (string[index] != '\0') {
        `$INSTANCE_NAME`_WriteData(string[index]);
        index++;
    }
}

void `$INSTANCE_NAME`_PrintByte(unsigned char byte) {
    char string[3];
    string[0] = NibbleToAscii(byte >> 4);
    string[1] = NibbleToAscii(byte & 0x0F);
    string[2] = '\0';
    
    `$INSTANCE_NAME`_PrintString(string);
}