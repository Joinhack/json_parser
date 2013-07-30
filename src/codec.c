#include "codec.h"

int utf8_to_ucs2 (const unsigned char * input, const unsigned char ** end_ptr) {
  *end_ptr = input;
  if (input[0] == 0)
    return -1;
  if (input[0] < 0x80) {
    * end_ptr = input + 1;
    return input[0];
  }
  if ((input[0] & 0xE0) == 0xE0) {
    if (input[1] == 0 || input[2] == 0)
      return -1;
    * end_ptr = input + 3;
    return
        (input[0] & 0x0F)<<12 |
        (input[1] & 0x3F)<<6  |
        (input[2] & 0x3F);
  }
  if ((input[0] & 0xC0) == 0xC0) {
    if (input[1] == 0)
      return -1;
    * end_ptr = input + 2;
    return
        (input[0] & 0x1F)<<6  |
        (input[1] & 0x3F);
  }
  return -1;
}


int ucs2_to_utf8 (int ucs2, unsigned char * utf8) {
  if (ucs2 < 0x80) {
    utf8[0] = ucs2;
    utf8[1] = '\0';
    return 1;
  }
  if (ucs2 >= 0x80  && ucs2 < 0x800) {
    utf8[0] = (ucs2 >> 6)   | 0xC0;
    utf8[1] = (ucs2 & 0x3F) | 0x80;
    utf8[2] = '\0';
    return 2;
  }
  if (ucs2 >= 0x800 && ucs2 < 0xFFFF) {
    utf8[0] = ((ucs2 >> 12)       ) | 0xE0;
    utf8[1] = ((ucs2 >> 6 ) & 0x3F) | 0x80;
    utf8[2] = ((ucs2      ) & 0x3F) | 0x80;
    utf8[3] = '\0';
    return 3;
  }
  return -1;
}