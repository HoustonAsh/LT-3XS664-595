#include "Display.h"


const uint8_t Display::charValue[128] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, Special::_Minus, 1, 1,
  Digits::_0,
  Digits::_1,
  Digits::_2,
  Digits::_3,
  Digits::_4,
  Digits::_5,
  Digits::_6,
  Digits::_7,
  Digits::_8,
  Digits::_9,
  1, 1, 1, 1, 1, 1, 1,
  Letters::Letter_A,
  Letters::Letter_B,
  Letters::Letter_C,
  Letters::Letter_D,
  Letters::Letter_E,
  Letters::Letter_F,
  Letters::Letter_G,
  Letters::Letter_H,
  Letters::Letter_I,
  Letters::Letter_J,
  Letters::Letter_K,
  Letters::Letter_L,
  Letters::Letter_M,
  Letters::Letter_N,
  Letters::Letter_O,
  Letters::Letter_P,
  Letters::Letter_Q,
  Letters::Letter_R,
  Letters::Letter_S,
  Letters::Letter_T,
  Letters::Letter_U,
  Letters::Letter_V,
  Letters::Letter_W,
  Letters::Letter_X,
  Letters::Letter_Y,
  Letters::Letter_Z,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1
};


Display::Display(uint8_t _enable, uint8_t _clock, uint8_t _data) {
  enable_pin = _enable;
  clock_pin = _clock;
  data_pin = _data;
}


void Display::setup() {
  pinMode(enable_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);

  cls();
}

/* Clock signal generator */
void Display::generateCLK() {
  digitalWrite(clock_pin, LOW);
  digitalWrite(clock_pin, HIGH);
  digitalWrite(clock_pin, LOW);
}

/* Enable signal generator */
void Display::generateEnable() {
  digitalWrite(enable_pin, LOW);
  digitalWrite(enable_pin, HIGH);
  digitalWrite(enable_pin, LOW);
}

/* Set initial pins state */
void Display::init() {
  digitalWrite(enable_pin, HIGH);
  digitalWrite(clock_pin, LOW);
  digitalWrite(data_pin, LOW);
}


void Display::print(uint8_t charCode) {
  init();
  for (int i = 0; i < 8; ++i) {
    digitalWrite(this->data_pin, charCode & bit(i));
    generateCLK();
    generateEnable();
  }
  init();
}

/* Clear screen */
Display& Display::cls() {
  clearRows();
  apply();
  return *this;
}

/* Clear state of all rows */
Display& Display::clearRows() {
  str1[0] = str2[0] = str3[0] = '\0';
  return *this;
}

/* Print updated rows */
Display& Display::apply() {
  static uint8_t rows[SLOTS];
  memset(rows, 1, SLOTS);
  translate(str1, rows, 0, ROW1_SLOTS);
  translate(str2, rows, ROW1_SLOTS, ROW1_SLOTS + ROW2_SLOTS);
  translate(str3, rows, ROW1_SLOTS + ROW2_SLOTS, SLOTS);

  for (int i = SLOTS - 1; i >= 0; --i) print(rows[i]);

  return *this;
}

/* Convert string to 7-segment char byte*/
size_t Display::translate(char* const str, uint8_t* buf, uint8_t l, uint8_t r) {
  size_t strLen = 0;
  uint8_t start = l;
  for (char* c = str; *c != '\0'; ++c) {
    if (*c == '.' || *c == ',') {
      if (l != start) {
        bitClear(buf[l - 1], 0);
        ++strLen;
      }
      continue;
    }
    if (l >= r) break;
    buf[l] = charValue[toUpperCase(*c)];
    ++strLen;
    ++l;
  }

  return strLen;
}


Display& Display::updateRows(String s1, String s2, String s3, Align a1, Align a2, Align a3) {
  updateRow(0, s1, a1);
  updateRow(1, s2, a2);
  updateRow(2, s3, a3);
  return *this;
}


Display& Display::updateRow(int indx, String s, Align a) {
  if (indx < 0 || indx > 2) return *this;
  memset(str[indx], ' ', strLen[indx]);
  int it = 0;
  int ln = 0;

  for (unsigned int i = 0; i < s.length(); ++i) {
    if (s[i] == '.' || s[i] == ',') {
      str[indx][it] = s[i];
      while (s[i] == '.' || s[i] == ',')
        if ((++i) == s.length()) break;
      --i;
      ++it;
      continue;
    }

    str[indx][it] = s[i];
    ++it;
    ++ln;
    if (it == strLen[indx]) break;
  }

  str[indx][it] = '\0';

  if (ln >= rowLen[indx] || a == Align::Left)
    return *this;

  String st;
  int sl = rowLen[indx] - ln;
  if (a == Align::Center) sl /= 2;
  while (sl--) st += " ";
  st += str[indx];
  memcpy(str[indx], st.c_str(), st.length() + 1);

  return *this;
}


Display& Display::clearRow(uint8_t i) { str[i][0] = '\0'; return *this; }