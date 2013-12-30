// Jose Javier Gonzalez Ortiz
// Libreria para el control de ejecucion de efectos en el cubo mediante el modulo UART
// parser.h

#ifndef _PARSER_H
#define _PARSER_H

//Librerias utilizadas
#include <stdint.h>     // Para el uso de uint8_t
#include <stdlib.h>     // Para atoi
#include "cubo.h"
#include "effect.h"


#define EFFECTS_ID  'E'
#define EFFECTS_NUM  30
#define GAMES_ID    'G'



void parse_message(char* code);
void send_periodo(void);
void parse_effect(char* code);
void initEffects(void);


#endif