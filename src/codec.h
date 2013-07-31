#ifndef CODEC_H
#define CODEC_H

#ifdef __cplusplus
extern "C" {
#endif

int ucs2_to_utf8 (int ucs2, unsigned char * utf8);
int utf8_to_ucs2 (const unsigned char * input, const unsigned char ** end_ptr);

#ifdef __cplusplus
}
#endif


#endif
