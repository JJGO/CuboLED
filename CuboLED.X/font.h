// Jose Javier Gonzalez Ortiz
// Libreria para el manejo de caracteres alfanumericos en el cubo LED
// font.h

#ifndef _FONT_H
#define _FONT_H

#define MESSAGE_SIZE 64
#define ASCII_OFFSET 0x20




#include "cubo.h"
#include <string.h>

void    font_effect_standard_push_message   (uint8_t reset);
void    font_effect_broadway_message        (uint8_t reset);
void    font_effect_slide_message           (uint8_t reset);

void    setMessage                          (char* str);


extern const uint8_t ascii[128-ASCII_OFFSET][8];
extern const uint8_t greek[58][8]; 
extern const uint8_t hiragana[96][8];   
extern const uint8_t extlatin[96][8];  


#endif