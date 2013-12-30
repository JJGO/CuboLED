
#include "effect.h"


void* current_effect;

// ----------------------------------- PROTOTIPOS -----------------------------------------

void effect_launcher(void);
void effect_animate(void*);
void effect_repeat(void*,uint8_t);

void draw_cube(uint8_t edge,uint8_t x,uint8_t y,uint8_t z);
void effect_animate_cube(uint8_t reset);
void effect_expand_cube(uint8_t reset);
void effect_rain(uint8_t reset);
void ring(int l, int z);
void effect_crossing_piramids(uint8_t reset);
void set_oblique(int d);
void effect_spin(uint8_t* config);
void random_fill(void);

// ----------------------------------- FUNCIONES ------------------------------------------

void effect_launch(void* effect)
{
    *effect(true);      // resetea el efecto
    current_effect = effect;
    period = -1;
}

void effect_animate(void* effect, uint8_t iterations)
{
    *effect(true);
    current_effect = effect;
    period = iterations;
    
}

void effect_quit(void)
{
    clearCube();
    current_effect = &effect_empty;
}

void effect_empty(void){
    
}



/*Nombre: draw_cube
 * Descripcion: Dibuja un cubo de lado edge con la esquina inferior izquierda de coordenadas x,y,z
 * Argumentos: edge - tamaño del lado
               x,y,z - coordenadas de la esquina inferior izquierda
 * Valor devuelto: Ninguno*/
void draw_cube(uint8_t edge,uint8_t x,uint8_t y,uint8_t z)
{
    uint8_t config;
    // Aristas en eje x
    uint8_t pos = edge-1;
    config = (0xff>>(N-edge))<<x;
    putAxis(X,  y,          z,      config);
    putAxis(X,  y,          z+pos,  config);
    putAxis(X,  y+pos,      z,      config);
    putAxis(X,  y+pos,      z+pos,  config);
    // Aristas en eje y
    config = (0xff>>(N-edge))<<y;
    putAxis(Y,  x,          z,      config);
    putAxis(Y,  x,          z+pos,  config);
    putAxis(Y,  x+pos,      z,      config);
    putAxis(Y,  x+pos,      z+pos,  config);
    // Aristas en eje z
    config = (0xff>>(N-edge))<<z;
    putAxis(Z,  x,          y,      config);
    putAxis(Z,  x,          y+pos,  config);
    putAxis(Z,  x+pos,      y,      config);
    putAxis(Z,  x+pos,      y+pos,  config);
}

/*Nombre: effect_rain
 * Descripcion: Ejecuta un efecto que consiste en expandir y comprimir un cubo de forma aleatoria contra las esquinas
                Requiere de drawCube()
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/
void effect_animate_cube(uint8_t reset)
{

    static int x = 0, y = 0, z = 0;
    static int i = 0, j = 0;
    static char growing = true;
    const char min = 3,max = 8;

    if(reset)
    {
        growing = true;
        i = j = x = y = z = 0;
        return;
    }

    if(growing){
        i++;
        if(i > max){
            i = max;
            growing = false;

            j = rand()&0x7;;
            x = (j) & 0x1;
            y = (j>>1) & 0x1;
            z = (j>>2) & 0x1;
        }
    }else{
        i--;
        if(i < min){
            i = min;
            growing = true;
            
        }
    }
    clearCube();
    draw_cube(i,x*(8-i),y*(8-i),z*(8-i));    
}

/*Nombre: effect_rain
 * Descripcion: Ejecuta un efecto que consiste en expandir y comprimir un cubo de forma periodica
                Requiere de drawCube()
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/
void effect_expand_cube(uint8_t reset)
{
    static int size = 2;
    static char growing = true;
    uint8_t corner = 0; 

    if(reset)
    {
        size = 2;
        growing = true;
        return;
    }

    if(growing ){
        size +=2;
        if(size > 8)
        {
            size = 8;
            growing = false;
        }
        
    }else if(!growing){
        size -=2;
        if(size < 2){
            size = 2;
            growing = true;
        }
            
    }
    corner = 4-size/2;
    clearCube();
    draw_cube(size,corner,corner,corner);
}


/*Nombre: effect_rain
 * Descripcion: Ejecuta un efecto de lluvia aleatoria en el cubo
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/
void effect_rain(uint8_t reset)
{
    static uint8_t generate = true;
    uint8_t y,z,drops;

    if(reset)
    {
        generate = true;
        clearCube();
        return;
    }

    for(z = 0; z < N-1 ; z++)
    {
        putPlane(Z,z,getPlane(Z,z+1));
    }

    if(generate)
    {
        clearLayer(7);
        drops = rand()%4;
        for(y = 0; y < drops; y++)
        {
            setVoxel(rand()%8,rand()%8,N-1);
        }
        generate = false;
    }
    else
        generate = true;
}


/*Nombre: ring
 * Descripcion: Crea un anillo de lado l
 * Argumentos: l - tamaño del lado
               z - plano en el que se pone el anillo
 * Valor devuelto: Ninguno*/
void ring(int l, int z)
{
    int a=4-l;
    putAxis(X, a, z, (0xFF>>2*a)<<a);
    putAxis(X, 7-a, z, (0xFF>>2*a)<<a);
    putAxis(Y, a, z, (0xFF>>2*a)<<a);
    putAxis(Y, 7-a, z, (0xFF>>2*a)<<a);
}

/*Nombre: effect_crossing_piramids
 * Descripcion: Ejecuta un efecto de piramides que se desplazan y se cruzan en el eje Z (requiere de ring() )
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/
void effect_crossing_piramids(uint8_t reset)
{
    static uint8_t j;
    uint8_t i,z;

    if(reset)
    {   
        j = 0;
        return;
    }

    clearCube();

    j++;
    if(j >= 12)
        j=0;
    z=j;
    for(i=1;i<=4;i++)
    {
        z = j-(i-1);
        if(z<0)
        {
            z=0;
        }else if(z>7){
            z=7;
        }
        ring(i,z);
        ring(i,7-z);
    }
}


/*
//9 Pasar los LEDs de un lado al contrario de forma aleatoria
void RandomMoveVertical(void)
{
    int i,j,axis,d;
    for(i=0; i<=7; i++)
    {
        for(j=0; j<=7; j++)
        {
            axis=getAxis(Z,i,j);
            if(test(axis,0)){
                srand (time (NULL));
                d=rand()%4;
                if(d!=0)
                   putAxis(Z,i,j, axis<<1);
            }else if(test(axis,7));
            else{
                putAxis(Z,i,j, axis<<1);
            }


        }

    }
}
*/

/*Nombre: set_oblique
 * Descripcion: Ejecuta un efecto de lluvia aleatoria en el cubo
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/
void set_oblique(int d)
{
    int i,j,k;

    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            for(k=0;k<8;k++)

                if(i+j+k==d)
                    setVoxel(i,j,k);
        }
    }
}

void effect_spin(uint8_t* config)
{

}



void random_fill(void)
{
    static int i = 0;
    uint8_t x,y,z;
    
    while(i<512){
        x = mod(rand());
        y = mod(rand());
        z = mod(rand());
        if(!voxel(x,y,z)){
            setVoxel(x,y,z);
           i++;
        }
    }

}



