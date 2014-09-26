#ifndef SO2002A_H
#define SO2002A_H

#ifndef __SAM3X8E__ 
#error "Not Arduino Due"
#error "Remove this with care"
#endif

#include "Print.h"

/* 
 * Macros for users
 */
#define SO2002A_CHARACTER_ROM_A 0x00
#define SO2002A_CHARACTER_ROM_B 0x04
#define SO2002A_CHARACTER_ROM_C 0x08

#define SO2002A_DISABLE_FADE_OUT_AND_BLINKING 0b00000000
#define SO2002A_ENABLE_FADE_OUT_MODE          0b00100000
#define SO2002A_ENABLE_BLINKING_MODE          0b00110000


/*
 * Macros for internal Use
 */
#define SO2002A_COMMAND_CLEAR_DISPLAY              0b00000001
#define SO2002A_COMMAND_RETURN_HOME                0b00000010
#define SO2002A_COMMAND_ENTRY_MODE_SET             0b00000100
#define SO2002A_COMMAND_DISPLAY_ON_OFF             0b00001000
#define SO2002A_COMMAND_CURSOR_OR_DISPLAY_SHIFT    0b00010000
#define SO2002A_COMMAND_FUNCTION_SET               0b00100000
#define SO2002A_COMMAND_SET_CGRAM_ADDRESS          0b01000000
#define SO2002A_COMMAND_SET_DDRAM_ADDRESS          0b10000000
#define SO2002A_COMMAND_SET_SCROLL_QUANTITY        0b10000000
#define SO2002A_COMMAND_SET_CONTRAST_CONTROL       0b10000001
#define SO2002A_COMMAND_SET_FADE_OUT_AND_BLINKING  0b00100011

#define SO2002A_COMMAND_FUNCTION_SELECTION_B    0b01110010
#define SO2002A_COMMAND_OLED_ENABLE_SD          0b01111001
#define SO2002A_COMMAND_OLED_DISABLE_SD         0b01111000

#define SO2002A_ENTRY_MODE_SHIFT_ON               (~SO2002A_ENTRY_MODE_SHIFT_OFF)
#define SO2002A_ENTRY_MODE_SHIFT_OFF                0b00000001
#define SO2002A_ENTRY_MODE_DIRECTION_LEFT_TO_RIGHT 0b00000010
#define SO2002A_ENTRY_MODE_DIRECTION_RIGHT_TO_LEFT (~SO2002A_ENTRY_MODE_DIRECTION_LEFT_TO_RIGHT)

#define SO2002A_DISPLAY_MODE_BLINK_ON    0b00000001
#define SO2002A_DISPLAY_MODE_BLINK_OFF   (~SO2002A_DISPLAY_MODE_BLINK_ON)
#define SO2002A_DISPLAY_CURSOR_ON        0b00000010
#define SO2002A_DISPLAY_CURSOR_OFF       (~SO2002A_DISPLAY_CURSOR_ON)
#define SO2002A_DISPLAY_MODE_DISPLAY_ON  0b00000100
#define SO2002A_DISPLAY_MODE_DISPLAY_OFF (~SO2002A_DISPLAY_MODE_DISPLAY_ON)

#define SO2002A_FUNCTION_EXT_IS_ON         0b00000001
#define SO2002A_FUNCTION_EXT_IS_OFF        (~SO2002A_FUNCTION_EXT_IS_ON)
#define SO2002A_FUNCTION_EXT_RE_ON         0b00000010
#define SO2002A_FUNCTION_EXT_RE_OFF        (~SO2002A_FUNCTION_EXT_RE_ON)
#define SO2002A_FUNCTION_DOUBLE_HEIGHT_ON  0b00000100
#define SO2002A_FUNCTION_DOUBLE_HEIGHT_OFF (~SO2002A_FUNCTION_DOUBLE_HEIGHT_ON)
#define SO2002A_FUNCTION_2LINE_MODE_ON     0b00001000
#define SO2002A_FUNCTION_2LINE_MODE_OFF    (~SO2002A_FUNCTION_2LINE_MODE_ON)

#define SO2002A_SCROLLQUANTITY_MASK     0b00111111
#define SO2002A_FADE_OUT_INTERVAL_MASK  0b00001111

#define SO2002A_SHIFT_CONTROL_ON 0b00001000
#define SO2002A_SHIFT_RIGHT      0b00000100
#define SO2002A_SHIFT_LEFT       0b00000000


class SO2002A : public Print {
public:
  SO2002A(uint8_t rs, uint8_t rw, uint8_t e, 
       uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3, 
       uint8_t db4, uint8_t db5, uint8_t db6, uint8_t db7);
      
  void begin(uint8_t columns, uint8_t rows);
  void clear();
  void home();
  void setCursor(uint8_t x, uint8_t y);
  void cursor();
  void noCursor();
  void blink();
  void noBlink();
  void display();
  void noDisplay();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void autoscroll();
  void noAutoscroll();
  void leftToRight();
  void rightToLeft();
  void createChar(uint8_t location, uint8_t charmap[]);

  void singleHeightFont();
  void doubleHeightFont();
  void setScrollQuantity(uint8_t dot);
  void selectCharacterRom(uint8_t font);
  void setContrastControl(uint8_t contrast);
  void setFadeOutAndBlinking(uint8_t mode, uint8_t interval);
  
  virtual size_t write(uint8_t);
  
  using Print::write;
private:
  uint8_t columns;      // width
  uint8_t rows;         // height
  uint8_t rsPin;        // Register Select
  uint8_t rwPin;        // Read/Write
  uint8_t ePin;         // Enable Signal
  uint8_t dbPin[8];     // Data Bit 0-8
  uint8_t entryMode;
  uint8_t displayMode;
  uint8_t functionMode;

  void setEntryMode();
  void setDisplayMode();
  void setFunctionMode();
  void setFunctionMode(uint8_t mode);
  void oledEnableSd();
  void oledDisableSd();
  void setCgramAddress(uint8_t address);
  void setDdramAddress(uint8_t address);
  void writeCommand(int rs, int data);
  void pulseEnable();
};

#endif  
