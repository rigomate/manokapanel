/*
 * mano_font.h
 *
 *  Created on: 2017. dec. 18.
 *      Author: mester
 */


#include <stdint.h>

typedef struct {
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;

typedef struct {
     long int code;
     const tImage *image;
     } tChar;
typedef struct {
     int length;
     const tChar *chars;
     } tFont;

extern const tFont ManoFont;
