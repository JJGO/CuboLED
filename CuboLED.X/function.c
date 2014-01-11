// Jose Javier Gonzalez Ortiz
// Copyright ©2013-2014, Jose Javier Gonzalez Ortiz. All rights reserved.

// Libreria de funciones para manipular sobre el CuboLED
// function.c




#include "function.h"

// ----------------------------------- PROTOTIPOS -----------------------------------------

void        setVoxel        (int8_t x,  int8_t y,   int8_t z);
void        clearVoxel      (int8_t x,  int8_t y,   int8_t z);
void        toggleVoxel     (int8_t x,  int8_t y,   int8_t z);
void        putVoxel        (int8_t x,  int8_t y,   int8_t z,   uint8_t b);
uint8_t     getVoxel        (int8_t x,  int8_t y,   int8_t z);

void        clearLayer      (int8_t z);
void        setLayer        (int8_t z);
void        clearCube       (void);
void        setCube         (void);
void        putAxis         (uint8_t dim, int8_t  coord1, int8_t   coord2, uint8_t config);
void        putPlane        (uint8_t dim, int8_t  coord,  uint8_t* config);
uint8_t     getAxis         (uint8_t dim, int8_t  coord1, int8_t   coord2);
uint8_t*    getPlane        (uint8_t dim, uint8_t coord);
void        fillPlane       (uint8_t dim, uint8_t coord,  uint8_t  config);
void        shiftCube       (uint8_t dim, uint8_t dir,    uint8_t  closed);

point       Point           (uint8_t x, uint8_t y, uint8_t z);
uint8_t     getPoint        (point p);
void        setPoint        (point p);
void        clearPoint      (point p);
void        togglePoint     (point p);
uint8_t     pointEquals     (point a, point p);
point       sumPoints       (point a, point b);
point       getRandomPoint  (void);


// ----------------------------------- FUNCIONES -----------------------------------------



/* Nombre: setVoxel
 * Descripción: Funcion para activar el voxel de coordenadas x, y, z
 * Argumentos: coordenadas x,y,z
 * Valor devuelto: Ninguno */ 

void setVoxel(int8_t x,int8_t y,int8_t z)
{
    setV(mod(x),mod(y),mod(z));
}

/* Nombre: clearVoxel
 * Descripción: Funcion para desactivar el voxel de coordenadas x, y, z
 * Argumentos: coordenadas x,y,z
 * Valor devuelto: Ninguno */ 

void clearVoxel(int8_t x,int8_t y,int8_t z)
{
    clearV(mod(x),mod(y),mod(z));
}

/* Nombre: toggleVoxel
 * Descripción: Funcion para conmutar el voxel de coordenadas x, y, z
 * Argumentos: coordenadas x,y,z
 * Valor devuelto: Ninguno */ 

void toggleVoxel(int8_t x,int8_t y,int8_t z)
{
    toggleV(mod(x),mod(y),mod(z));
}

/* Nombre: putVoxel 
 * Descripción: Funcion que pone el valor b en el voxel de coordenadas x, y, z
 * Argumentos: coordenadas x,y,z
 * Valor devuelto: Ninguno */ 

void putVoxel(int8_t x,int8_t y,int8_t z,uint8_t b)
{
    //putV(mod(x),mod(y),mod(z),b&0x1);
    
    if(b)
        setV(mod(x),mod(y),mod(z));
    else
        clearV(mod(x),mod(y),mod(z));
}

/* Nombre: getVoxel 
 * Descripción: Funcion que devuelve el bit de coordenadas x, y, z
 * Argumentos: coordenadas x,y,z
 * Valor devuelto: Valor discreto del voxel 0 - 1 */ 

uint8_t getVoxel(int8_t x,int8_t y,int8_t z)
{
    return voxel(mod(x),mod(y),mod(z));
}

/* Nombre: clearLayer
 * Descripción: Funcion para borrar una capa del cubo
 * Argumentos:  capa z
 * Valor devuelto: Ninguno */ 

void clearLayer(int8_t z)
{
    z = mod(z);
    uint8_t y;
    for(y = 0; y < N ; y++)
    {
        voxel[z][y] = 0x00;
    }
}

/* Nombre: setLayer
 * Descripción: Funcion para activar una capa del cubo
 * Argumentos:  capa z
 * Valor devuelto: Ninguno */ 

void setLayer(int8_t z)
{
    z = mod(z);
    uint8_t y;
    for(y = 0; y < N ; y++)
    {
        voxel[z][y] = 0xff;
    }
}


/* Nombre: clearCube
 * Descripción: Funcion que borra todo el cubo
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void clearCube(void)
{
    uint8_t z;
    for(z = 0 ; z < N ; z++)
        clearLayer(z);
}

/* Nombre: setCube
 * Descripción: Funcion que activa todo el cubo
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void setCube(void)
{
    uint8_t z;
    for(z = 0 ; z < N ; z++)
        setLayer(z);
}



/* Nombre: putAxis
 * Descripción: Funcion de activacion de un eje (X,Y,Z) del cubo en funcion de una configuracion 
 * Argumentos:  dim - dimension del eje 
                    X -> X
                    Y -> Y
                    Z -> Z
                coord1 - primera coordenada necesaria 
                    X -> coordenada en Y
                    Y -> coordenada en X
                    Z -> Coordenanda en X
                coord2 - segunda coordenada necesaria 
                    X -> coordenada en Z
                    Y -> coordenada en Z
                    Z -> Coordenanda en Y
                config - byte que contiene los valores del eje en cuestion
                    X -> configuracion del eje X
                    Y -> configuracion del eje Y
                    Z -> configuracion del eje Z
 * Valor devuelto: Ninguno */ 

 void putAxis(uint8_t dim, int8_t coord1, int8_t coord2, uint8_t config)
{
    uint8_t y,z;
    coord1 = mod(coord1);
    coord2 = mod(coord2);

    if(dim == X)
            voxel[coord2][coord1] = config;
    else if(dim == Y)
        for(y=0 ; y < N ; y++)
            putVoxel(coord1,y,coord2,test(config,y));
    else if(dim == Z)
        for(z=0 ; z < N ; z++)
            putVoxel(coord1,coord2,z,test(config,z));   
}

/* Nombre: putPlane
 * Descripción: Funcion de activacion de un plano perpendicular al eje (X,Y,Z) del cubo en funcion de una configuracion 
 * Argumentos:  dim - dimension del plano 
                    X -> X
                    Y -> Y
                    Z -> Z
                coord1 - coordenada del plano  
                    X -> coordenada en X
                    Y -> coordenada en Y
                    Z -> Coordenanda en Z
                config* - vector de bytes que contiene los valores del eje en cuestion
                    X -> vector de las configuraciones en el eje Y con indice en Z
                    Y -> vector de las configuraciones en el eje X con indice en Z
                    Z -> vector de las configuraciones en el eje X con indice en Y
 * Valor devuelto: Ninguno */ 

void putPlane(uint8_t dim, int8_t coord, uint8_t* config)
{
    uint8_t y,z;
    coord = mod(coord);
    if(dim == X)
    {   // Caso especial los config son Yrows
        for(z = 0 ; z < N ; z++)
            putAxis(Y,coord,z,config[z]);
    }
    else if(dim == Y)
    {   // Los config son Xrows
        for(z = 0 ; z < N ; z++)
            putAxis(X,coord,z,config[z]);
    }
    else if(dim == Z)
    {   // Los config son Xrows
        for(y = 0 ; y < N ; y++)
            putAxis(X,y,coord,config[y]);
    }
}

/* Nombre: getAxis
 * Descripción: Funcion de obtencion de la configuracion actual de un eje (X,Y,Z) del cubo 
 * Argumentos:  dim - dimension del eje 
                    X -> X
                    Y -> Y
                    Z -> Z
                coord1 - primera coordenada necesaria 
                    X -> coordenada en Y
                    Y -> coordenada en X
                    Z -> Coordenanda en Y
                coord2 - segunda coordenada necesaria 
                    X -> coordenada en Z
                    Y -> coordenada en Z
                    Z -> Coordenanda en Y
 * Valor devuelto:  
                config - byte que contiene los valores del eje en cuestion
                    X -> configuracion del eje X
                    Y -> configuracion del eje Y
                    Z -> configuracion del eje Z*/

uint8_t getAxis(uint8_t dim, int8_t coord1, int8_t coord2)
{
    uint8_t i,axis = 0;
    coord1 = mod(coord1);
    coord2 = mod(coord2);

    if(dim == X)
        axis = voxel[coord2][coord1];
    else if(dim == Y)
        for(i=0 ; i < N ; i++)
            if(voxel(coord1,i,coord2))
                set(axis,i);
    else if(dim == Z)
        for(i=0 ; i < N ; i++)
            if(voxel(coord1,coord2,i))
                set(axis,i);
    return axis;
}

/* Nombre: getPlane
 * Descripción: Funcion de obtencion de la configuracion actual de un plano perpendicular al eje (X,Y,Z) del cubo 
 * Argumentos:  dim - dimension del plano 
                    X -> X
                    Y -> Y
                    Z -> Z
                coord1 - coordenada del plano  
                    X -> coordenada en X
                    Y -> coordenada en Y
                    Z -> Coordenanda en Z
                
 * Valor devuelto: 
                config* - vector de bytes que contiene los valores del eje en cuestion
                    X -> vector de las configuraciones en el eje Y con indice en Z
                    Y -> vector de las configuraciones en el eje X con indice en Z
                    Z -> vector de las configuraciones en el eje X con indice en Y
                    */

uint8_t* getPlane(uint8_t dim, uint8_t coord)
{
    static uint8_t config[N];
    uint8_t y,z;
    coord = mod(coord);
    if(dim == X)
    {   // Caso especial los config son Yrows
        for(z = 0 ; z < N ; z++)
            config[z] = getAxis(Y,coord,z);
    }
    else if(dim == Y)
    {   // Los config son Xrows
        for(z = 0 ; z < N ; z++)
            config[z] = getAxis(X,coord,z);
    }
    else if(dim == Z)
    {   // Los config son Xrows
        for(y = 0 ; y < N ; y++)
            config[y] = getAxis(X,y,coord);
    }

    return config;
}

/* Nombre: fillPlane
 * Descripción: Funcion de rellenado de un plano utilizando una cofiguracion dada para todos los ejes
 * Argumentos:  dim - dimension del plano 
                    X -> X
                    Y -> Y
                    Z -> Z
                coord1 - coordenada del plano  
                    X -> coordenada en X
                    Y -> coordenada en Y
                    Z -> Coordenanda en Z
                config - configuracion del eje a replicar
                
 * Valor devuelto: Ninguno*/

void fillPlane(uint8_t dim, uint8_t coord, uint8_t config)
{
    uint8_t y,z;
    coord = mod(coord);
    if(dim == X)
    {   // Caso especial los config son Yrows
        for(z = 0 ; z < N ; z++)
            putAxis(Y,coord,z,config);
    }
    else if(dim == Y)
    {   // Los config son Xrows
        for(z = 0 ; z < N ; z++)
            putAxis(X,coord,z,config);
    }
    else if(dim == Z)
    {   // Los config son Xrows
        for(y = 0 ; y < N ; y++)
            putAxis(X,y,coord,config);
    }
}


/* Nombre: shiftCube
 * Descripción: Funcion para desplazar el contenido del cupo
 * Argumentos:  dim - dimension de la rotacion (X,Y,Z)
                dir - indica la direccion (0 IZQUIERDA, 1 DERECHA)
                closed - indica si se considera un cubo cerrado
 * Valor devuelto: Ninguno */ 

void shiftCube(uint8_t dim,uint8_t dir, uint8_t closed)
{
    uint8_t i = 0;
    uint8_t buffer[8],*config;
    
    if(dir == 0){ 
        config = getPlane(dim,N-1);
        for(i = 0; i < N ; i++)
        {
            buffer[i] = config[i];
        }
        
        for(i = N-2; i < N ; i--){
           putPlane(dim,i+1,getPlane(dim,i));
        }
        if(closed)
        {
            putPlane(dim,0,buffer);
        }
        
    }else{
        config = getPlane(dim,N);
        for(i = 0; i < N ; i++)
        {
            buffer[i] = config[i];
        }
        for(i = 0; i < N-1 ; i++){
           putPlane(dim,i,getPlane(dim,i+1));
        }
        if(closed)
        {
            putPlane(dim,N-1,buffer); 
        }
        
    }

}

/* Nombre: Point
 * Descripción: Constructor de la clase Punto
 * Argumentos:  x - coordenada en x
                y - coordenada en y
                z - coordenada en z
 * Valor devuelto: p - la estructura de tipo point con esos valores */ 

point Point(uint8_t x, uint8_t y, uint8_t z)
{
    point p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

/* Nombre: setPoint
 * Descripción: Funcion para activar el voxel de coordenadas p.x, p.y, p.z
 * Argumentos:  p - estructura de tipo punto que almacena las coordenadas
 * Valor devuelto: Ninguno */ 

void setPoint(point p)
{
    setV(p.x,p.y,p.z);
}

/* Nombre: clearPoint
 * Descripción: Funcion para desactivar el voxel de coordenadas p.x, p.y, p.z
 * Argumentos:  p - estructura de tipo punto que almacena las coordenadas
 * Valor devuelto: Ninguno */ 

void clearPoint(point p)
{
    clearV(p.x,p.y,p.z);
}

/* Nombre: togglePoint
 * Descripción: Funcion para conmutar el voxel de coordenadas p.x, p.y, p.z
 * Argumentos:  p - estructura de tipo punto que almacena las coordenadas
 * Valor devuelto: Ninguno */ 

void togglePoint(point p)
{
    toggleV(p.x,p.y,p.z);
}

/* Nombre: setPoint
 * Descripción: Funcion obtener el valor del voxel de coordenadas p.x, p.y, p.z
 * Argumentos:  p - estructura de tipo punto que almacena las coordenadas
 * Valor devuelto: Valor del voxel de coordenadas p.x, p.y, p.z */ 

uint8_t getPoint(point p)
{
    return voxel(p.x,p.y,p.z);
}

/* Nombre: pointEquals
 * Descripción: Funcion para comprobar la igualdad coordenada a coordenada de dos puntos
 * Argumentos:  a - estructura de tipo punto que almacena las coordenadas del primer punto
                b - estructura de tipo punto que almacena las coordenadas del segundo punto
 * Valor devuelto: valor booleano de igualdad */ 

uint8_t pointEquals(point a, point b)
{
    if(a.x == b.x)
        if(a.y == b.y)
            if(a.z == b.z)
                return true;
    return false;
}

/* Nombre: sumPoints
 * Descripción: Funcion para sumar dos puntos coordenada a coordenada (uno debera representar un vector)
 * Argumentos:  a - estructura de tipo punto que almacena las coordenadas del primer punto
                b - estructura de tipo punto que almacena las coordenadas del segundo punto (direccion)
 * Valor devuelto: valor booleano de igualdad */ 

point sumPoints(point a, point b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

/* Nombre: pointEquals
 * Descripción: Funcion para comprobar la igualdad coordenada a coordenada de dos puntos
 * Argumentos:  a - estructura de tipo punto que almacena las coordenadas del primer punto
                b - estructura de tipo punto que almacena las coordenadas del segundo punto
 * Valor devuelto: valor booleano de igualdad */ 

point getRandomPoint(void)
{
    return Point(rand()&0x07,rand()&0x07,rand()&0x07);
}

