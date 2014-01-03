// Jose Javier Gonzalez Ortiz
// Libreria para la definicion de efectos ejecutables por el CuboLED
// effect.h

#ifndef _EFFECT_H
#define _EFFECT_H

//Librerias utilizadas 
#include "cubo.h"                       // Libreria base para interactuar con el cubo. Incluye tambien function.h
// #include <stdint.h>                     // para el uso de int8_t
#include <stdlib.h>                     // Para la generacion de algoritmos pseudo-aleatorios
#include <math.h>                       // Para el uso de funciones matematicas

//Librerias externas de efectos
#include "font.h"                       // Libreria de efectos con cadenas de caracteres alfanumericos
#include "snake.h"                      // Libreria de juegos 

#define TMIN            N*TREFRESH
#define FACTOR_DEF      2

// Tipos externos

typedef void (*peffect)(uint8_t);       //puntero a efecto que toma el parametro de reset

// Globales

extern uint8_t analog_period;

// Definicion de funciones publicas 

void        effect_launch               (peffect effect);
void        effect_reset                (void);
void        effect_repeat               (peffect effect, int8_t iterations);
void        effect_launch_second        (peffect effect, int8_t iterations);
void        effect_clean                (void);
void        effect_quit                 (void);
void        effect_second_quit          (void);
void        effect_empty                (uint8_t a);
void        effect_launcher             (void);

uint16_t    getPeriodo                  (void);
void        setPeriodo                  (uint16_t);
void        setFactor                   (uint8_t factor);

void        effect_draw_cube            (uint8_t  reset);
void        effect_animate_cube         (uint8_t  reset);
void        effect_expand_cube          (uint8_t  reset);
void        effect_rain                 (uint8_t  reset);
void        effect_crossing_piramids    (uint8_t  reset);
void        effect_spin                 (uint8_t* config);
void        effect_random_fill          (uint8_t  reset);
void        effect_sweep_plane          (uint8_t  reset);
void        effect_random_move          (uint8_t  reset);
void        effect_cascade              (uint8_t  reset);
void        effect_random_move_vertical (uint8_t  reset);
void        effect_random_fragment      (uint8_t  reset);
void        effect_wave                 (uint8_t  reset);
void        effect_water_drop           (uint8_t  reset);
void        effect_water_drop_2         (uint8_t  reset);
void        effect_spiral               (uint8_t  reset);
void        effect_lysa3d               (uint8_t  reset);
void        effect_test                 (uint8_t  reset);
void        effect_octahedron           (uint8_t  reset);
void        effect_diagonal             (uint8_t  reset);


#endif