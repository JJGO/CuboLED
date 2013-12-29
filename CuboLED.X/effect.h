#ifndef _EFFECT_H
#define _EFFECT_H

//Librerias utilizadas 
#include "cubo.h"
#include <stdint.h>       // para el uso de int8_t
#include <stdlib.h>
#include <math.h>

void drawCube(uint8_t edge,uint8_t x,uint8_t y,uint8_t z);
void ring(int l, int z);
void setOblique(int d);
void animateCube(int r);
void expandCube(void);
void rain(void);
void randomFill(void);
void circle(float rad,uint8_t z);

#endif