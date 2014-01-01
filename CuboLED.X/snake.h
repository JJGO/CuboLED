// Jose Javier Gonzalez Ortiz
// Libreria para la definicion de juegos ejecutables por el CuboLED
// snake.h

#ifndef _SNAKE_H
#define _SNAKE_H

//Librerias utilizadas 
#include "cubo.h"
#include <stdint.h>       // para el uso de int8_t

#define MAX_LENGTH  64
#define MIN_LENGTH  3
#define X_0         0
#define Y_0         0
#define Z_0         0

// Definicion de funciones publicas

void    init_snake  (void);
void    game_snake  (uint8_t reset);

#endif