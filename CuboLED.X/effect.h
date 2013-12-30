#ifndef _EFFECT_H
#define _EFFECT_H

//Librerias utilizadas 
#include "cubo.h"
#include <stdint.h>       // para el uso de int8_t
#include <stdlib.h>
#include <math.h>

//Librerias extrnas de efectos
#include "font.h"

typedef void (*peffect)(uint8_t);

void        effect_launch               (peffect effect);
void        effect_repeat               (peffect effect, uint8_t iterations);
void        effect_quit                 (void);
void        effect_empty(uint8_t a);
void        effect_launcher             (void);
uint16_t    getPeriodo                  (void);
void        setFactor                   (uint8_t factor);

void        draw_cube                   (uint8_t edge,uint8_t x,uint8_t y,uint8_t z);
void        ring                        (int l, int z);
void        set_oblique                 (int d);

void        effect_animate_cube         (uint8_t reset);
void        effect_expand_cube          (uint8_t reset);
void        effect_rain                 (uint8_t reset);
void        effect_crossing_piramids    (uint8_t reset);
void        effect_spin                 (uint8_t* config);
void        random_fill                 (void);


#endif