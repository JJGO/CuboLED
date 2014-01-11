// Jose Javier Gonzalez Ortiz
// Copyright ©2013-2014, Jose Javier Gonzalez Ortiz. All rights reserved.

// Libreria para la definicion de juegos ejecutables por el CuboLED
// snake.h

#ifndef _SNAKE_H
#define _SNAKE_H

//Librerias utilizadas 
#include "cubo.h"


#define MAX_LENGTH  64
#define MIN_LENGTH  4
#define X_0         3
#define Y_0         3
#define Z_0         0

// Definicion de funciones publicas

void    init_snake  (void);
void    game_snake  (uint8_t reset);

#endif