#ifndef _EFFECT_H
#define _EFFECT_H

//Librerias utilizadas 
#include "cubo.h"
#include <stdint.h>       // para el uso de int8_t
#include <stdlib.h>
#include <math.h>

//Librerias extrnas de efectos

void effect_launcher(void);
void effect_animate(void*);
void effect_repeat(void*,uint8_t);

void draw_cube(uint8_t edge,uint8_t x,uint8_t y,uint8_t z);
void effect_animate_cube(uint8_t reset);
void effect_expand_cube(uint8_t reset);
void effect_rain(uint8_t reset);
void ring(int l, int z);
void effect_crossing_piramids(uint8_t reset);
void set_oblique(int d);
void effect_spin(uint8_t* config);
void random_fill(void);

#endif