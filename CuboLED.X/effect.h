#ifndef _EFFECT_H
#define _EFFECT_H

//Librerias utilizadas 
#include "cubo.h"
#include <stdint.h>       // para el uso de int8_t
#include <stdlib.h>
#include <math.h>

//Librerias extrnas de efectos

void draw_cube(uint8_t edge,uint8_t x,uint8_t y,uint8_t z);
void ring(int l, int z);
void set_oblique(int d);
void random_fill(void);

void expand_cube(uint8_t reset);
void rain(uint8_t reset);
void animate_cube(uint8_t reset);
void animate_ring(uint8_t reset);

#endif