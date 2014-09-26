#include "SO2002A.h"
#include "Arduino.h"

SO2002A::SO2002A(uint8_t rs, uint8_t rw, uint8_t e, 
            uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3, 
            uint8_t db4, uint8_t db5, uint8_t db6, uint8_t db7) {
  rsPin = rs;
  rwPin = rw;
  ePin  = e;
  dbPin[0] = db0;
  dbPin[1] = db1;
  dbPin[2] = db2;
  dbPin[3] = db3;
  dbPin[4] = db4;
  dbPin[5] = db5;
  dbPin[6] = db6;
  dbPin[7] = db7;

  pinMode(rsPin, OUTPUT);
  pinMode(rwPin, OUTPUT);
  pinMode(ePin, OUTPUT);
  
  for(int i = 0; i < 8; i++) {
    pinMode(dbPin[i], OUTPUT);
  }
}

void SO2002A::begin(uint8_t columns, uint8_t rows) {
  this->columns = columns;
  this->rows = rows;
  
  entryMode =    0b00000010;
  displayMode =  0b00000000;
  functionMode = 0b00000000;

  noDisplay();
  clear();
  display();
  
  setDisplayMode();
  setFunctionMode();
}

void SO2002A::clear() {
  writeCommand(0, SO2002A_COMMAND_CLEAR_DISPLAY);
  delayMicroseconds(1520);
}

void SO2002A::home() {
  writeCommand(0, SO2002A_COMMAND_RETURN_HOME);
  delayMicroseconds(1520);
}

void SO2002A::setCursor(uint8_t col, uint8_t row) {
  int row_begin[] = {0, 32};
  setDdramAddress(col + row_begin[row] );
}

void SO2002A::cursor() {
  displayMode |= SO2002A_DISPLAY_CURSOR_ON;
  setDisplayMode();
}

void SO2002A::noCursor() {
  displayMode &= SO2002A_DISPLAY_CURSOR_OFF;
  setDisplayMode();
}

void SO2002A::blink() {
  displayMode |= SO2002A_DISPLAY_MODE_BLINK_ON;
  setDisplayMode();
}

void SO2002A::noBlink() {
  displayMode &= SO2002A_DISPLAY_MODE_BLINK_OFF;
  setDisplayMode();
}

void SO2002A::display() {
  displayMode |= SO2002A_DISPLAY_MODE_DISPLAY_ON;
  setDisplayMode();
}

void SO2002A::noDisplay() {
  displayMode &= SO2002A_DISPLAY_MODE_DISPLAY_OFF;
  setDisplayMode();
}

void SO2002A::scrollDisplayLeft() {
  writeCommand(0, SO2002A_COMMAND_CURSOR_OR_DISPLAY_SHIFT | SO2002A_SHIFT_CONTROL_ON | SO2002A_SHIFT_LEFT);
  delayMicroseconds(37);
}

void SO2002A::scrollDisplayRight() {
  writeCommand(0, SO2002A_COMMAND_CURSOR_OR_DISPLAY_SHIFT | SO2002A_SHIFT_CONTROL_ON | SO2002A_SHIFT_RIGHT);
  delayMicroseconds(37);
}

void SO2002A::autoscroll() {
  entryMode |= SO2002A_ENTRY_MODE_SHIFT_OFF;
  setEntryMode();
}

void SO2002A::noAutoscroll() {
  entryMode &= SO2002A_ENTRY_MODE_SHIFT_ON;
  setEntryMode();
}

void SO2002A::leftToRight() {
  entryMode |= SO2002A_ENTRY_MODE_DIRECTION_LEFT_TO_RIGHT;
  setEntryMode();
}

void SO2002A::rightToLeft() {
  entryMode &= SO2002A_ENTRY_MODE_DIRECTION_RIGHT_TO_LEFT;
  setEntryMode();
}

void SO2002A::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7;
  writeCommand(0, SO2002A_COMMAND_SET_CGRAM_ADDRESS | (location << 3));
  delayMicroseconds(37);
  for (int i = 0; i < 8; i++) {
    write(charmap[i]);
    delayMicroseconds(37);
  }
}

void SO2002A::singleHeightFont() {
  functionMode &= SO2002A_FUNCTION_DOUBLE_HEIGHT_OFF;
  setFunctionMode();
}

void SO2002A::doubleHeightFont() {
  functionMode |= SO2002A_FUNCTION_DOUBLE_HEIGHT_ON;
  setFunctionMode();
}

void SO2002A::setScrollQuantity(uint8_t dot) {
  setFunctionMode(functionMode | SO2002A_FUNCTION_EXT_RE_ON);
  writeCommand(0, SO2002A_COMMAND_SET_SCROLL_QUANTITY | (dot & SO2002A_SCROLLQUANTITY_MASK));
  //writeCommand(0, 0b10000011);
  delayMicroseconds(37);
  setFunctionMode(functionMode & SO2002A_FUNCTION_EXT_RE_OFF);
}

void SO2002A::selectCharacterRom(uint8_t font) {
  setFunctionMode(functionMode | SO2002A_FUNCTION_EXT_RE_ON);
  writeCommand(0, SO2002A_COMMAND_FUNCTION_SELECTION_B);
  writeCommand(1, font);
  delayMicroseconds(37);
  setFunctionMode(functionMode & SO2002A_FUNCTION_EXT_RE_OFF);
  clear();
}

void SO2002A::setContrastControl(uint8_t contrast) {
  setFunctionMode(functionMode | SO2002A_FUNCTION_EXT_RE_ON);
  oledEnableSd();
  writeCommand(0, SO2002A_COMMAND_SET_CONTRAST_CONTROL);
  writeCommand(0, contrast);
  delayMicroseconds(37);
  oledDisableSd();
  setFunctionMode(functionMode & SO2002A_FUNCTION_EXT_RE_OFF);
}

void SO2002A::setFadeOutAndBlinking(uint8_t mode, uint8_t interval) {
  setFunctionMode(functionMode | SO2002A_FUNCTION_EXT_RE_ON);
  oledEnableSd();
  writeCommand(0, SO2002A_COMMAND_SET_FADE_OUT_AND_BLINKING);
  writeCommand(0, mode | (interval & SO2002A_FADE_OUT_INTERVAL_MASK));
  delayMicroseconds(37);
  oledDisableSd();
  setFunctionMode(functionMode & SO2002A_FUNCTION_EXT_RE_OFF);
}

size_t SO2002A::write(uint8_t value) {
  writeCommand(1, value);
  delayMicroseconds(37);
  return 1;
}

void SO2002A::setEntryMode() {
  writeCommand(0, SO2002A_COMMAND_ENTRY_MODE_SET | entryMode);
  delayMicroseconds(37);
}

void SO2002A::setDisplayMode() {
  writeCommand(0, SO2002A_COMMAND_DISPLAY_ON_OFF | displayMode);
  delayMicroseconds(37);
}

void SO2002A::setFunctionMode() {
  writeCommand(0, SO2002A_COMMAND_FUNCTION_SET | functionMode);
  delayMicroseconds(37);
}

void SO2002A::setFunctionMode(uint8_t mode) {
  writeCommand(0, SO2002A_COMMAND_FUNCTION_SET | mode);
  delayMicroseconds(37);
}

void SO2002A::oledEnableSd() {
  writeCommand(0, SO2002A_COMMAND_OLED_ENABLE_SD);
}

void SO2002A::oledDisableSd() {
  writeCommand(0, SO2002A_COMMAND_OLED_DISABLE_SD);
}

void SO2002A::setCgramAddress(uint8_t address) {
  writeCommand(0, SO2002A_COMMAND_SET_CGRAM_ADDRESS | address);
  delayMicroseconds(37);
}

void SO2002A::setDdramAddress(uint8_t address) {
  writeCommand(0, SO2002A_COMMAND_SET_DDRAM_ADDRESS | address);
  delayMicroseconds(37);
}


/*
 * Low Level Control
 */
void SO2002A::writeCommand(int rs, int data) {
  digitalWrite(rsPin, rs);
  digitalWrite(rwPin, LOW);
  
  for (int i = 0; i < 8; i++, data >>= 1) {
    digitalWrite(dbPin[i], 1 & data);
  }

  pulseEnable();
  digitalWrite(rwPin, HIGH);
}

void SO2002A::pulseEnable() {
  digitalWrite(ePin, LOW);
  delayMicroseconds(1);
  digitalWrite(ePin, HIGH);
  delayMicroseconds(1);
  digitalWrite(ePin, LOW);
}

