#ifndef CODEC_H
#define CODEC_H

int ucs2_to_utf8 (int ucs2, unsigned char * utf8);
int utf8_to_ucs2 (const unsigned char * input, const unsigned char ** end_ptr);

#endif
