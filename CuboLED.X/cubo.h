// Jose Javier Gonzalez Ortiz
// Libreria para el manejo del cubo LED
// cubo.h

#ifndef _CUBO_H
#define _CUBO_H

//Librerias utilizadas 
#include "driver.h"
#include "stdint.h"			// para el uso de int8_t
#include "math.h"
//#include "font8x8_basic.h"


// Macros para utilizar máscaras de forma cómoda
#define setV(a,b,c)   	(voxel[c][b] |= (0x01) << a)
#define clearV(a,b,c)   (voxel[c][b] &= ~((0x01) << a))
#define toggleV(a,b,c)	(voxel[c][b] = (0x01) << a)
#define putV(a,b,c,p)		(voxel[c][b] ^= (p<<a))
#define voxel(a,b,c) 	((voxel[c][b] >> a) & 0x01)


// Macros para la defincion de constantes
#define N 8
//#define mod(a) (a < 0 ? a%N+N : a%N)
#define mod(a) (a & 0x07)

#define TREFRESH 	20		//periodo de refresco de las capas en decimas de milisegundo
#define TSCLK		2		//periodo del reloj serie en microsegundos
#define TCLEAR		2		//en microsegundos (con 2 funciona pero shading)


#define _SDI	PORTBbits.RB11		
#define _SDO	PORTBbits.RB10	
#define _SCK	PORTBbits.RB7	
#define _SS 	PORTBbits.RB6	

#define X 		1
#define Y 		2
#define Z 		3

#define true 1
#define false 0

struct point 
{
	uint8_t x;
	uint8_t y;
	uint8_t z;
};


// Variables globales
//uint8_t
static uint8_t voxel[N][N];

// Prototipos de las funciones publicas

void 		cubeInit	(void);
void 		setVoxel	(int8_t x, int8_t y, int8_t z);
void 		clearVoxel	(int8_t x, int8_t y, int8_t z);
void 		toggleVoxel	(int8_t x, int8_t y, int8_t z);
void 		putVoxel	(int8_t x, int8_t y, int8_t z, uint8_t b);
uint8_t		getVoxel	(int8_t x, int8_t y, int8_t z);
void 		clearLayer	(int8_t z);
void 		setLayer	(int8_t z);
void 		clearCube	(void);
void 		setCube		(void);
void 		Refresh		(void);
void 		putAxis		(uint8_t dim, int8_t  coord1, int8_t   coord2, uint8_t config);
void 		putPlane	(uint8_t dim, int8_t  coord,  uint8_t* config);
uint8_t		getAxis		(uint8_t dim, int8_t  coord1, int8_t   coord2);
uint8_t* 	getPlane	(uint8_t dim, uint8_t coord);

void setOblique(int d);
void drawCube(uint8_t edge,uint8_t x,uint8_t y,uint8_t z);
void ring(int l, int z);
void animateCube(int);

#endif
