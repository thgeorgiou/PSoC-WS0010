/**
 * @file ws0010.h
 * @author Thanasis Georgiou
 * @brief Defines functions and constants for the WS0010 component.
 *
 * This component is designed to be mostly compatible with the stock Character LCD component. There are
 * some quirks here and there but transition should be painless.
 */
    
#ifndef WS0010_`$INSTANCE_NAME`_H
#define WS0010_`$INSTANCE_NAME`_H

// Control commands
#define LCD_CLEAR_DISPLAY 0x01      /** Clears the display **/
#define LCD_RETURN_HOME 0x02        /** Returns the cursor to the home position */
#define LCD_FUNCTION_SET 0x28
#define LCD_SET_CGRAM_ADDR 0x40     /** Set a specific character RAM address, use to add custom characters */
#define LCD_SET_DDRAM_ADDR 0x80     /** Set a specific DDRAM address */

// flags for display entry mode
#define LCD_ENTRY_MODE_SET 0x04             /** Use with entry mode flags to change how the text is inserted */
#define LCD_ENTRY_RIGHT 0x00                /** Right-to-left text */
#define LCD_ENTRY_LEFT 0x02                 /** Left-to-right text*/
#define LCD_ENTRY_SHIFT_INCREMENT 0x01      /** 'Right justify' text */
#define LCD_ENTRY_SHIFT_DECREMENT 0x00      /** 'Left justify' text */

// flags for display on/off control
#define LCD_DISPLAY_CONTROL 0x08    /** Use to turn the display on/off and toggle cursor/blink. */
#define LCD_DISPLAY_ON 0x04         /** Turn the display on */
#define LCD_DISPLAY_OFF 0x00        /** Turn the display off */
#define LCD_CURSOR_ON 0x02          /** Turn the cursor visible */
#define LCD_CURSOR_OFF 0x00         /** Turn the cursor invisible */
#define LCD_BLINK_ON 0x01           /** Turn on cursor blinking */
#define LCD_BLINK_OFF 0x00          /** Turn off cursor blinking */

// flags for display/cursor shift
#define LCD_CURSOR_SHIFT 0x10   /** Use with LCD_DISPLAY_MOVE to scroll the display without chaning the RAM. */
#define LCD_DISPLAY_MOVE 0x08   /** Use with LCD_CURSOR_SHIFT to scroll the display without chaning the RAM. */
#define LCD_CURSOR_MOVE 0x00    /** Move cursor */
#define LCD_MOVE_RIGHT 0x04     /** Scroll the display right */
#define LCD_MOVE_LEFT 0x00      /** Scroll the display left */

// Power and display modes
#define LCD_POWER_ON 0x07       /** Use with LCD_CURSOR_SHIFT to turn on the power */
#define LCD_POWER_OFF 0x03      /** Use with LCD_CURSOR_SHIFT to turn off the power */
#define LCD_GRAPHIC_MODE 0x0B   /** Use with LCD_CURSOR_SHIFT to go in graphic mode (don't forget power) */
#define LCD_CHARACTER_MODE 0x03 /** Use with LCD_CURSOR_SHIFT to go in graphic mode (don't forget power) */

// flags for function set
#define LCD_8BIT_MODE   0x10
#define LCD_4BIT_MODE   0x00
#define LCD_JAPANESE    0x00
#define LCD_EUROPEAN_I  0x01
#define LCD_RUSSIAN     0x02
#define LCD_EUROPEAN_II 0x03

// High/Low
#define HIGH    1
#define LOW     0

// R/W Pin
#define `$INSTANCE_NAME`_WriteMode() `$INSTANCE_NAME`_Pin_RW_Write(LOW);
#define `$INSTANCE_NAME`_ReadMode() `$INSTANCE_NAME`_Pin_RW_Write(HIGH);

// RS Pin
#define `$INSTANCE_NAME`_DataMode() `$INSTANCE_NAME`_Pin_RS_Write(HIGH);
#define `$INSTANCE_NAME`_ControlMode() `$INSTANCE_NAME`_Pin_RS_Write(LOW);

// ======== Low level commands ========
/**
 * Send one nibble (half a byte) to the display.
 * @param nibble  The nibble to send. The last 4 bits will be ignored since this function will only sends 4 bits.
 * @see `$INSTANCE_NAME`_SendByte()
 */
void `$INSTANCE_NAME`_SendNibble(unsigned char nibble);

/**
 * Send a whole byte to the display. Since we are in 4-bit mode, it will first send the high nibble and then
 * the low one.
 * @param byte  The byte to send
 * @see `$INSTANCE_NAME`_SendNibble()
 * @see `$INSTANCE_NAME`_SendCommand()
 * @see `$INSTANCE_NAME`_SendData()
 */
void `$INSTANCE_NAME`_SendByte(unsigned char byte);

/**
 * Send a command to the display. Commands are 1-byte long.
 * @param cmd   The command to send.
 * @see `$INSTANCE_NAME`_SendData()
 */
void `$INSTANCE_NAME`_WriteControl(unsigned char cmd);

/**
 * Send data to the display. Use this to write a single character at the cursor's position.
 * @param data  The character to write
 * @see `$INSTANCE_NAME`_SendCommand()
 */
void `$INSTANCE_NAME`_WriteData(unsigned char data);
/**
 * Alias for stock LCD compatibility,
 * @see `$INSTANCE_NAME`_SendData()
 */
#define `$INSTANCE_NAME`_PutChar(data) `$INSTANCE_NAME`_SendData(data);

/**
 * Stalls code execution until the display is ready for the next command.
 */
void `$INSTANCE_NAME`_IsReady(void);

// ======== High level commands ========
/**
 * Prepares the display for use. For now, just an alias for `$INSTANCE_NAME`_Init().
 * @see `$INSTANCE_NAME`_Init()
 */
void `$INSTANCE_NAME`_Start(void);

/**
 * Initializes the display for use. This will:
 * - Restart the display.
 * - Set the display in 4-bit mode
 * - Turn the display off and then clear the contents.
 * - Turn it back on.
 */
void `$INSTANCE_NAME`_Init(void);

/**
 * Sets the cursor to the given position.
 * @param row       The row to set. If larger than the display, it will reset to the first row.
 * @param column    The column position.
 */
void `$INSTANCE_NAME`_Position(uint8_t column, uint8_t row);

/**
 * Turns off the display. In sleep mode, the display should draw around 0.8mA.
 * @see `$INSTANCE_NAME`_Wakeup()
 * @warning This command ignores 'show cursow' and 'blink cursor' settings. If these must persist, you must manually put the display
 *          to sleep using your own LCD_DISPLAY_CONTROL command. 
 */
void `$INSTANCE_NAME`_Sleep();

/**
 * Turns on the display.
 * @see `$INSTANCE_NAME`_Sleep()
 */
void `$INSTANCE_NAME`_Wakeup();

/**
 * Write a string at the current cursor position.
 * @param string String to write.
 */
void `$INSTANCE_NAME`_PrintString(char *string);

/** 
 * Write a byte at the current cursor position in hexdecimal notation.
 * @param byte Byte to write.
 */
void `$INSTANCE_NAME`_PrintByte(unsigned char byte);

// For stock LCD compatibility
#define `$INSTANCE_NAME`_ClearDisplay() `$INSTANCE_NAME`_WriteControl(LCD_CLEAR_DISPLAY);
#define `$INSTANCE_NAME`_DisplayOn() `$INSTANCE_NAME`_WriteControl(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
#define `$INSTANCE_NAME`_DisplayOff() `$INSTANCE_NAME`_WriteControl(LCD_DISPLAY_CONTROL | LCD_DISPLAY_OFF);

#endif // `$INSTANCE_NAME`_H