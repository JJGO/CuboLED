// Jose Javier Gonzalez Ortiz
// Libreria de funciones para manipular sobre el CuboLED
// function.h


#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "cubo.h"


// Macros para operar sobre el CuboLED de forma directa

#define setV(a,b,c)     (voxel[c][b] |= (0x01) << a)
#define clearV(a,b,c)   (voxel[c][b] &= ~((0x01) << a))
#define toggleV(a,b,c)  (voxel[c][b] = (0x01) << a)
#define putV(a,b,c,p)   (voxel[c][b] ^= (p<<a))
#define voxel(a,b,c)    ((voxel[c][b] >> a) & 0x01)


// Definicion de constantes para el BlackBoxing de los ejes
#define X           1
#define Y           2
#define Z           3

// Permite el CuboLED sea proyectivamente cerrado
#ifdef _MOD
#define mod(a) (a & 0x07)
#endif

#ifndef _MOD
#define mod(a) (a)
#endif

#define inrange(a)  ((a) == ((a) & 0x07))

//Declaracion de estructuras

//nombre puro de la estructura point
struct pointTag;

//Typedef para evitar el keyword struct cada vez que haya una ocurrencia de point
typedef struct pointTag
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
}point;

// Decalracion de funciones publicas
void        setVoxel        (int8_t x, int8_t y, int8_t z);
void        clearVoxel      (int8_t x, int8_t y, int8_t z);
void        toggleVoxel     (int8_t x, int8_t y, int8_t z);
void        putVoxel        (int8_t x, int8_t y, int8_t z, uint8_t b);
uint8_t     getVoxel        (int8_t x, int8_t y, int8_t z);
void        clearLayer      (int8_t z);
void        setLayer        (int8_t z);
void        clearCube       (void);
void        setCube         (void);
void        Refresh         (void);
void        putAxis         (uint8_t dim, int8_t  coord1, int8_t   coord2, uint8_t config);
void        putPlane        (uint8_t dim, int8_t  coord,  uint8_t* config);
uint8_t     getAxis         (uint8_t dim, int8_t  coord1, int8_t   coord2);
uint8_t*    getPlane        (uint8_t dim, uint8_t coord);
    
void        shiftCube       (uint8_t dim, uint8_t dir,    uint8_t closed);
void        fillPlane       (uint8_t dim, uint8_t coord,  uint8_t config);

point       getRandomPoint  (void);
uint8_t     pointEquals     (point a, point p);
void        setPoint        (point p);
void        clearPoint      (point p);
uint8_t     getPoint        (point p);

uint8_t     count_neighboors(uint8_t x, uint8_t y, uint8_t z);

#endif
