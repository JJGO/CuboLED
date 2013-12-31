// Jose Javier Gonzalez Ortiz
// Libreria para el manejo del cubo LED
// cubo.h

#ifndef _CUBO_H
#define _CUBO_H

//Librerias utilizadas 

#include "p33FJ32MC202.h"   // Libreria de Microchip para poder utilizar la sintaxis C
#include "config.h"         // Fichero de configuracion para incializar el reloj y remapear los perifericos

#include <stdint.h>			// para el uso de int8_t

//Librerias externas para el uso del cubo
#include "function.h"
#include "effect.h"
#include "serial.h"

// Macros para la defincion de constantes
#define N           8

#define TREFRESH 	20		//periodo de refresco de las capas en decimas de milisegundo
#define TCLEAR		2		//en microsegundos 

#define true        1
#define false       0

//Macros del Timer1
#define MS                  FCY/1000        // Valor de PR1 para funcionar a milisegundos
#define DMS                 FCY/10000       // Valor de PR1 para funcionar a decimas de milisegundo
#define US                  FCY/1000000     // Valor de PR1 para funcionar a microsegundos

// Macros para utilizar máscaras de forma cómoda
#define clear(file,b)     (file &= ~((0x0001) << b))
#define set(file,b)       (file |= (0x0001) << b)
#define put(file,b,x)     (file ^= (x<<b))
#define toggle(file,b)    (file ^= (0x0001) << b)
#define test(file,b)      (file & (0x0001 << b))

//Macros de puertos
#define TRISA_Def   0xFFFF
#define TRISB_Def   0x0FFF

//Macros del detector de flanco
#define SWITCH      test(~PORTB,2)          //Pulsador en RB2

// Puertos del SPI
#define _SDI	    PORTBbits.RB11		
#define _SDO	    PORTBbits.RB10	
#define _SCK	    PORTBbits.RB7	
#define _SS 	    PORTBbits.RB6	

extern uint8_t voxel[N][N];

// Declaracion de funciones publicas
void        cubeInit    (void);
int         get_ad      (int canal);



#endif
