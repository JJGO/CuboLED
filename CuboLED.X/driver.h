// Jose Javier Gonzalez Ortiz
// Libreria de driver para el micro dsPIC33FJ32MC202
// driver.h

#ifndef _DRIVER_H
#define _DRIVER_H

//Librerias utilizadas 
#include "p33FJ32MC202.h"	// Libreria de Microchip para poder utilizar la sintaxis C
#include "config.h"			// Fichero de configuracion para incializar el reloj y remapear los perifericos
#include "uart.h"			// Libreria para el manejo del modulo serie

#include "cubo.h"

// Macros para utilizar máscaras de forma cómoda
#define clear(file,b)   (file &= ~((0x0001) << b))
#define set(file,b)     (file |= (0x0001) << b)
#define put(file,b,x)   (file ^= (x<<b))
#define toggle(file,b)  (file ^= (0x0001) << b)
#define test(file,b)    (file & (0x0001 << b))

//Macros de puertos
#define TRISA_Def 		0xFFFF
#define TRISB_Def 		0x0FFF

//Macros del Timer1
#define MS 				FCY/1000	    // Valor de PR1 para funcionar a milisegundos
#define DMS				FCY/10000 	    // Valor de PR1 para funcionar a decimas de milisegundo
#define US 				FCY/1000000     // Valor de PR1 para funcionar a microsegundos

//Macros del detector de flanco
#define SWITCH			test(~PORTB,2) 		//Pulsador en RB2


// Prototipos de las funciones publicas
void mainInit(void);

void setDigitalPorts(void);

void initTimer1(void);
void delay(int);
void delay_us(int);

void vTimer1(void);
void vTimer2(void);

void PWM1(void);

void EdgeDetect(int);
char watch_uart(void);

#endif
