// Jose Javier Gonzalez Ortiz
// Libreria para el control de ejecucion de efectos en el cubo mediante el modulo UART
// serial.h

#ifndef _SERIAL_H
#define _SERIAL_H

//Librerias utilizadas
#include <stdint.h>         // Para el uso de uint8_t
#include <stdlib.h>         // Para atoi

#include "cubo.h"
#include "uart.h"           // Libreria para el manejo del modulo serie

#define ESC                 0x1B

#define BUFFER_SIZE         64
#define NUM_EFFECTS         40 
    
#define EFFECT_ID           'E'
#define FONT_ID             'F'
#define GAMES_ID            'G'
    
#define NUM_COMMAND         7
    
#define NO_COMMAND          0 
#define FWD_COMMAND         1
#define REV_COMMAND         2 
#define LEFT_COMMAND        3 
#define RIGHT_COMMAND       4 
#define UP_COMMAND          5 
#define DOWN_COMMAND        6   

#define UP_KEY_COMMAND      7 
#define DOWN_KEY_COMMAND    8
#define RIGHT_KET_COMMAND   9 
#define LEFT_KEY_COMMAND   10 



extern uint8_t echo;

//Declaracion de funciones publicas

void    initEffects             (void);
char    watch_uart              (void);
void    parse_message           (char* code);
void    parse_effect            (char* code);
void    parse_font_effect       (char* code);
void    parse_command           (uint8_t message);
void    parse_escape_command    (uint8_t message);
uint8_t getCommand              (void);
void    cleanBuffer             (void);
    
void    send_str                (char* str);
void    send_int                (uint16_t i);
void    send_hex                (uint16_t i);
void    send_periodo            (void);

#endif