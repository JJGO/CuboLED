// Jose Javier Gonzalez Ortiz
// Libreria para el control de ejecucion de efectos en el cubo mediante el modulo UART
// serial.h

#ifndef _SERIAL_H
#define _SERIAL_H

//Librerias utilizadas
#include <stdint.h>     // Para el uso de uint8_t
#include <stdlib.h>     // Para atoi

#include "cubo.h"
#include "uart.h"       // Libreria para el manejo del modulo serie


#define EFFECTS_ID  'E'
#define EFFECTS_NUM  30
#define GAMES_ID    'G'

void initEffects(void);
char watch_uart(void);
void parse_message(char* code);
void send_periodo(void);
void parse_effect(char* code);
void initEffects(void);


#endif