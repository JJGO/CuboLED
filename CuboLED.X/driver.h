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
#define MS 				40000	//Valor de PR1 para funcionar a milisegundos
#define DMS				4000 	//Valor de PR1 para funcionar a decimas de milisegundo
#define US 				40

//Macros del detector de flanco
#define SWITCH			test(~PORTB,2) 		//Pulsador en RB2

// Variables globales

//Variables de virtual timers
static unsigned int ticksVT1 = 0;
static unsigned int periodoVT1 = 2000;

static unsigned int ticksVT2 = 0;
static unsigned int periodoVT2 = 7919;

static unsigned int ticksVT3 = 0;
static unsigned int periodoVT3 = 0;

//Variables de PWM
static unsigned int ticksPWM1 = 0;
static unsigned int dutyPWM1 = 0;
static unsigned int periodoPWM1 = 20;

static unsigned int ticksPWM2 = 0;
static unsigned int dutyPWM2 = 0;
static unsigned int periodoPWM2 = 0;

//Variables de PWMPer
static unsigned int ticksPWMPer1 = 0;
static unsigned int dutyPWMPer1;
static unsigned int periodoPWMPer1 = 0;

static unsigned int ticksPWMPer2 = 0;
static unsigned int dutyPWMPer2;
static unsigned int periodoPWMPer2 = 0;

//Variables de Edge detector


static unsigned int _REdge = 1;		// Habilita las acciones en flanco de subida
static unsigned int _FEdge = 0;		// Habilita las acciones en flanco de bajada


// Prototipos de las funciones publicas
void mainInit(void);

void setDigitalPorts(void);

void initTimer1(void);
void delay(int);
void delay_us(int);

void vTimer1(void);
void vTimer2(void);
void vTimer3(void);

void PWM1(void);
void PWM2(void);

void PWMPer1(int);
void PWMPer2(int);

void EdgeDetect(int);

char watchUART(void);

#endif
