// Jose Javier Gonzalez Ortiz
// Libreria para la definicion de juegos ejecutables por el CuboLED
// snake.h

#ifndef _SNAKE_H
#define _SNAKE_H

//Librerias utilizadas 
#include "cubo.h"
#include <stdint.h>       // para el uso de int8_t

#define PX  1            // Positive X direction
#define NX  2            // Positive X direction              
#define PY  3            // Positive Y direction
#define NY  4            // Positive Y direction
#define PZ  5            // Positive Z direction
#define NZ  6            // Positive Z direction

// Definicion de funciones publicas

void    init_snake  (void);
void    game_snake  (uint8_t reset);



#endif