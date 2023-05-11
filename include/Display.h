/*********************************************************************
  7 segment display schema:
 ┌── a ──┐
 |       |
 f       b
 |       |
 ├── g ──┤
 |       |
 e       c
 |       |
 └── d ──┴── h

 a - high bit
 h - low bit

**********************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

#define ROW1_SLOTS 6
#define ROW2_SLOTS 6
#define ROW3_SLOTS 4


#include <Arduino.h>
#include "SevenSegmentChars.h"

enum Align {
  Left,
  Center,
  Right,
};

class Display {
public:
  Display(uint8_t _enable, uint8_t _clock, uint8_t _data);

  Display& updateRows(
    String s1,
    String s2,
    String s3,
    Align a1 = Align::Left,
    Align a2 = Align::Left,
    Align a3 = Align::Left
  );
  Display& updateRow(int i, String str, Align a = Align::Left);
  Display& clearRows();
  Display& clearRow(uint8_t i);
  Display& apply();
  Display& cls();

  void setup();

protected:
  static const uint8_t SLOTS = ROW1_SLOTS + ROW2_SLOTS + ROW3_SLOTS;
  static const uint8_t STR1_LEN = (ROW1_SLOTS << 1);
  static const uint8_t STR2_LEN = (ROW2_SLOTS << 1);
  static const uint8_t STR3_LEN = (ROW3_SLOTS << 1);


private:
  inline void generateEnable() __attribute__((always_inline));
  inline void generateCLK() __attribute__((always_inline));
  inline void init() __attribute__((always_inline));
  void print(uint8_t charCode);
  size_t translate(char* const s, uint8_t* buf, uint8_t l, uint8_t r);

  static const uint8_t charValue[128];
  int enable_pin;
  int clock_pin;
  int data_pin;

  char str1[STR1_LEN + 1];
  char str2[STR2_LEN + 1];
  char str3[STR3_LEN + 1];

  char* const str[3] = { str1, str2, str3 };
  uint8_t strLen[3] = { STR1_LEN, STR2_LEN, STR3_LEN };
  uint8_t rowLen[3] = { ROW1_SLOTS, ROW2_SLOTS, ROW3_SLOTS };
};

#endif
