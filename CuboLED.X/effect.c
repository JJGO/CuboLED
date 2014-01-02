// Jose Javier Gonzalez Ortiz
// Libreria para la definicion de efectos ejecutables por el CuboLED
// effect.c

#include "effect.h"

// ------------------------------------ GLOBALES ------------------------------------------

static peffect   current_effect;         // Puntero al efecto actual que se esta ejecutando
static peffect   second_effect;          // Puntero al efecto secundario que se esta ejecutando mediante el efecto actual
static int16_t   counter = 0;            // Contador de iteraciones pendientes, si vale -1 es infinito
static uint8_t   second_active = false;  // flag para saber si se esta ejecutando un 

static uint16_t  ticks_effect = 0;       // Contador del timer para hacer la actualizacion de 
static uint16_t  periodo_effect = 1000;  //Periodo entre llamadas al efecto en milisegundos 
       uint8_t   analog_period = true;
static uint8_t   factor = 2;
// ----------------------------------- PROTOTIPOS -----------------------------------------

void        effect_launch               (peffect effect);
void        effect_reset                (void);
void        effect_repeat               (peffect effect, int8_t iterations);
void        effect_launch_second        (peffect effect, int8_t iterations);
void        effect_clean                (void);
void        effect_quit                 (void);
void        effect_second_quit          (void);
void        effect_empty                (uint8_t a);
void        effect_launcher             (void);

uint16_t    getPeriodo                  (void);
void        setPeriodo                  (uint16_t);
void        setFactor                   (uint8_t factor);

void        effect_animate_cube         (uint8_t  reset);
void        effect_expand_cube          (uint8_t  reset);
void        effect_rain                 (uint8_t  reset);
void        effect_crossing_piramids    (uint8_t  reset);
void        effect_spin                 (uint8_t* config);
void        effect_random_fill          (uint8_t  reset);
void        effect_sweep_plane          (uint8_t  reset);
void        effect_random_move          (uint8_t reset);
void        effect_cascade              (uint8_t reset);
void        effect_random_move_vertical (uint8_t reset);

void        draw_cube                   (uint8_t edge,uint8_t x,uint8_t y,uint8_t z);
void        ring                        (int l, int z);
void        set_oblique                 (int d);


// ----------------------------------- FUNCIONES ------------------------------------------

// ---------------------FUNCIONES DE ADMINISTRACION DE EFECTOS ----------------------------

/*Nombre: effect_launcher
 * Descripcion: Rutina de monitorizacion y ejecucion de los efectos, llama a los efectos y obtiene el periodo 
                a partir del potenciometro si esta habilitado 
 * Argumentos:  Ninguno
 * Valor devuelto: Ninguno*/

void effect_launcher(void)
{
    ticks_effect++;
    if(analog_period)
        periodo_effect = factor*get_ad(5)+TMIN; // si esta habilitado por el efecto el periodo es variable 
    if(ticks_effect >= periodo_effect)
    {
        ticks_effect = 0;
        if(second_active)
        {
            (*second_effect)(false);
            if(counter > 0){
                counter--;
            }else if(counter == 0){
                effect_second_quit();
            }
        }else
        {
            (*current_effect)(false);
            if(counter > 0){
                counter--;
            }else if(counter == 0){
                effect_quit();
            }
        }
    }
}

/*Nombre: effect_reset
 * Descripcion: Resetea el efecto actual
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno*/

void effect_reset(void)
{
    effect_second_quit();
    (*current_effect)(true);
}

/*Nombre: effect_launch
 * Descripcion: Resetea el efecto a lanzar y lo establece como el efecto actual de forma indefinida
 * Argumentos: effect - puntero al efecto a ejecutar
 * Valor devuelto: Ninguno*/

void effect_launch(peffect effect)
{
    effect_repeat(effect,-1);
}

/*Nombre: effect_repeat
 * Descripcion: Resetea el efecto a lanzar y lo establece como el efecto actual un cantidad de iteraciones proporcionada
 * Argumentos:  effect - puntero al efecto a ejecutar
                iterations - numero de iteraciones que el efecto ejecutara
 * Valor devuelto: Ninguno*/

void effect_repeat(peffect effect, int8_t iterations)
{
    if(current_effect != &effect_empty)
        effect_quit();
    (*effect)(true);    // resetea el efecto
    current_effect = effect;
    counter = iterations;
    
}

/*Nombre: effect_launch_second
 * Descripcion: Ejecuta un numero de iteraciones un efecto secundario, sin llegar a salir de un efecto superior o padre 
 * Argumentos: effect - puntero al efecto a ejecutar
               iterations - numero de iteraciones que el efecto ejecutara
 * Valor devuelto: Ninguno*/

void effect_launch_second(peffect effect, int8_t iterations)
{
    if(current_effect == &effect_empty)
        effect_repeat(effect,iterations);       // Si no tiene padre simplemente se lanza
    (*effect)(true);      // resetea el efecto
    second_effect = effect;
    second_active = true;
    counter = iterations;
}

/*Nombre: effect_clean
 * Descripcion: Limpia el cuboLED y sus variables asociadas, pero sin tocar el puntero a efecto actual
 * Argumentos:  Ninguno
 * Valor devuelto: Ninguno*/

void effect_clean(void)
{
    echo = true;
    analog_period = true;
    factor = FACTOR_DEF;
    clearCube();
}

/*Nombre: effect_quit
 * Descripcion: Sale del efecto actual y limpia el cubo LED
 * Argumentos:  Ninguno
 * Valor devuelto: Ninguno*/

void effect_quit(void)
{
    effect_second_quit();
    current_effect = &effect_empty;
}

/*Nombre: effect_second_quit
 * Descripcion: Sale del efecto secundario para volver al actual y limpiar el cubo LED
 * Argumentos:  Ninguno
 * Valor devuelto: Ninguno*/

void effect_second_quit(void)
{
    effect_clean();
    second_effect = &effect_empty;
    second_active = false;
}

/*Nombre: effect_empty
 * Descripcion: Efecto vacio para poder tener al cubo en idle mode
 * Argumentos:  Ninguno
 * Valor devuelto: Ninguno*/

void effect_empty(uint8_t a)
{
    
}

/*Nombre: getPeriodo
 * Descripcion: Funcion de obtencion del periodo actual para el facil setup de DEMOS 
 * Argumentos: Ninguno
 * Valor devuelto: El valor actual de la variable periodo entre los efectos*/

uint16_t getPeriodo(void)
{
    return periodo_effect;
}

/*Nombre: setPeriodo
 * Descripcion: Funcion de configuracion del periodo actual
                Posee una validacion del periodo minimo para evitar la aparicion de patrones Moire 
                Los cuales ocurren si periodo_effect < N * TREFRESH
 * Argumentos: El valor a fijar del periodo 
 * Valor devuelto: Ninguno*/

void setPeriodo (uint16_t periodo)
{
    if(periodo>TMIN){
        analog_period = false;
        periodo_effect = periodo;
    }
}

/*Nombre: setFactor
 * Descripcion: Funcion de configuracion del factor de velocidad
 * Argumentos: El valor a fijar del factor de velocidad aplicado a la lectura analogica
 * Valor devuelto: Ninguno*/

void setFactor(uint8_t f)
{
    factor = f;
}


// ------------------------------------ EFECTOS -------------------------------------------


/*Nombre: effect_animate_cube
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
        clearCube();
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
        clearCube();
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
    uint8_t y,drops;

    if(reset)
    {
        generate = true;
        clearCube();
        return;
    }

    shiftCube(Z,true,false);

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
    else{
        generate = true;
    }
}

/*Nombre: effect_cascade
 * Descripcion: Ejecuta un efecto de cascada aleatoria en el cubo
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/

 void effect_cascade(uint8_t reset)
{
    static point p;
    int8_t dx, dy;
    if(reset)
    {
        p.x = rand()%7;
        p.y = rand()%7;
        p.z = N-1;
        clearCube();
        return;
    }

    shiftCube(Z,true,false);
    // clearLayer(N-1);
    clearVoxel(p.x   ,p.y   ,N-1);
    clearVoxel(p.x+1 ,p.y   ,N-1);
    clearVoxel(p.x   ,p.y+1 ,N-1);
    clearVoxel(p.x+1 ,p.y+1 ,N-1);

    dx = rand()%3-1;
    dy = rand()%3-1;

    if((p.x+dx) < 0 || p.x+dx > N-1){
        dx = -dx;
    }
    
    if((p.y+dy) < 0 || p.y+dy > N-1){
        dy = -dy;
    }
    p.x += dx;
    p.y += dy;

    setVoxel(p.x   ,p.y   ,N-1);
    setVoxel(p.x+1 ,p.y   ,N-1);
    setVoxel(p.x   ,p.y+1 ,N-1);
    setVoxel(p.x+1 ,p.y+1 ,N-1);


}




/*Nombre: effect_crossing_piramids
 * Descripcion: Ejecuta un efecto de piramides que se desplazan y se cruzan en el eje Z 
                requiere de ring() 
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/

void effect_crossing_piramids(uint8_t reset)
{
    static uint8_t j;
    uint8_t i,z;

    if(reset)
    {   
        j = 0;
        clearCube();
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
        ring(i,z);          // Piramide ascendente
        ring(i,7-z);        // Piramide descendente
    }
}

void effect_random_move_vertical(uint8_t reset)
{
    static uint8_t x,y, up,leaps;
    uint8_t r;
    if(reset)
    {
        clearCube();
        for(x = 0; x < N; x++)
        {
            r = rand();
            for(y = 0; y < N; y++)
            {
                if((r>>y)&0x01){
                    setVoxel(x,y,0);
                }else{
                    setVoxel(x,y,N-1);
                }
            } 
        }
        leaps = N;
    }

    if(leaps >= N-1)
    {
        leaps = 0;
        x = rand()&0x07;
        y = rand()&0x07;
        up = getVoxel(x,y,0);
    }
    // if(up){
    //    putAxis(Z,x,y,getAxis(Z,x,y)<<1);
    // }else{
    //    putAxis(Z,x,y,getAxis(Z,x,y)>>1);
    // }
    leaps++;
    putAxis(Z,x,y,0x00);
    setVoxel(x,y,up ? leaps :(N-1)-leaps);


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


/*Nombre: effect_sweep_plane
 * Descripcion: Ejecuta un efecto de barrido de planos en X, Y ,Z
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/

void effect_sweep_plane(uint8_t reset)
{
    static uint8_t i,j;

    if(reset)
    {
        i = j = 0;
        clearCube();
        return;
    }

    if(i>=N){
        i = 0;
        j++;
        if(j>=6)
        {
            j=0;
        }
    }
    clearCube();
    fillPlane(j/2+1,j%2 ? N-1-i : i,0xff);

    i++;
}

/*Nombre: effect_random_move
 * Descripcion: Ejecuta un efecto de mover un punto de forma aleatoria que va rebotando contra las paredes
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/


void effect_random_move(uint8_t reset)
{   
    static int8_t count;
    static point p, dp;
    if(reset)
    {
        p = getRandomPoint();
        dp = Point(rand()%3-1,rand()%3-1,rand()%3-1);
        count = 0;
        clearCube();
        return;
    }   

    if(count >= 20)
    {
        count = 0;
        dp = Point(rand()%3-1,rand()%3-1,rand()%3-1);
    }

    if(!inrange(p.x+dp.x))
        dp.x = -dp.x;
    
    if(!inrange(p.y+dp.y))
        dp.y = -dp.y;

    if(!inrange(p.z+dp.z))
        dp.z = -dp.z;
    
    p = sumPoints(p,dp);

    clearCube();

    setPoint(p);

    count++;


}

/*Nombre: effect_random_fill
 * Descripcion: Ejecuta un efecto de rellenar el cubo de forma aleatoria
 * Argumentos: reset - parametro de reinicializacion del efecto
 * Valor devuelto: Ninguno*/

void effect_random_fill(uint8_t reset)
{
    static int i = 0;
    point p;

    if(reset)
    {   
        i = 0;
        clearCube();
    }

    if(i >= 512)
    {
        effect_random_fill(false);
    }

    while(1){
        p = getRandomPoint();
        if(!getPoint(p)){
            setPoint(p);
            i++;
            break;
        }
    }

}

void effect_spin(uint8_t* config)
{

}


// ---------------- FUNCIONES AUXILIARES PARA LA CREACION DE EFECTOS ----------------------

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

/*Nombre: ring
 * Descripcion: Crea un anillo de lado l
 * Argumentos: l - tamaño del lado
               z - plano en el que se pone el anillo
 * Valor devuelto: Ninguno*/
void ring(int l, int z)
{
    int a=4-l;
    putAxis(X, a,   z, (0xFF>>2*a)<<a);
    putAxis(X, 7-a, z, (0xFF>>2*a)<<a);
    putAxis(Y, a,   z, (0xFF>>2*a)<<a);
    putAxis(Y, 7-a, z, (0xFF>>2*a)<<a);
}

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

