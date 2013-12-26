// Jose Javier Gonzalez Ortiz
// Libreria para el manejo del cubo LED
// cubo.c 

#include "cubo.h"

// ----------------------------------- FUNCIONES -----------------------------------------

/* Nombre: cubeInit
 * Descripción: Funcion de incializacion completa del cubo LED
 * Argumentos: Nnguno
 * Valor devuelto: Ninguno */ 

void cubeInit(void)
{
    SPIinit();
    // UART Init
    clearCube();
}

/* Nombre: SPI_PinRemap
 * Descripción: Funcion especifica para el remapeado de puertos del SPI para la placa trainer de ICAI
 * Argumentos: Nnguno
 * Valor devuelto: Ninguno */ 

void SPI_PinRemap(void)
{
    // Unlock Registers
    __builtin_write_OSCCONL(OSCCON & ~(1<<6)); 

    // Configure Input Functions
    // (See Table 30-1)
    // Assign SPI1 Data SDI to pin RP11 (RB11)
    RPINR20bits.SDI1R = 11;

    // Configure Output Functions
    // (See Table 30-2)
    
    // Assign SSDO1 (Serial Data Out) to pin RP10 (RB10)
    // 00111
    RPOR5bits.RP10R = 0b00111;
    //Asign SCK1OUT (Serial clock out) to pin RP7 (RB7)
    // 01000
    RPOR3bits.RP7R  = 0b01000;
    // Assign SS1OUT (SPI1 Slave Select) to pin RP6 (RB6)
    //01001
    // N se usa el SS como talRPOR3bits.RP6R    = 0b01001;


    // Lock Registers
    __builtin_write_OSCCONL(OSCCON | (1<<6));
}

/* Nombre: SPIInit
 * Descripción: Funcion de incializacion del SPI para la comunicacion entre el cubo y el microsegundos
 * Argumentos: Nnguno
 * Valor devuelto: Ninguno */ 

void SPIinit(void)
{

    //PORT Remap And Tristate configuration
    SPI_PinRemap();
    set(TRISB,11);      // SDI1
    clear(TRISB,10);    // SD01
    clear(TRISB,7);     // SCK
    clear(TRISB,6);     // SS
    _SS = 1;            // /Clear a 1

    /*
    //SPI1CON1
    IFS0bits.SPI1IF = 0;    // Clear the Interrupt Flag
    IEC0bits.SPI1IE = 0;    // Disable the Interrupt

                            // SPI1CON1 Register Settings 
    SPI1CON1bits.DISSCK=0;  // InternalSerialClockisEnabled 
    SPI1CON1bits.DISSDO=0;  // SDO1 pin is controlled by the module
    SPI1CON1bits.MODE16=0;  // Communicationisword-wide(8bits)
    SPI1CON1bits.SMP = 0;   // Serial output data changes on transition
    SPI1CON1bits.CKE = 0;   // from Idle clock state to active clock state
    SPI1CON1bits.CKP = 0;   // Idle state for clock is a low level;
                            // active state is a high level

    SPI1CON1bits.SSEN = 0;  // Se utiza para clear por lo que se deshabilita

    SPI1CON1bits.SPRE = 000 // Secondary prescaler 8:1
    SPI1CON1bits.PPRE = 10  // Primary prescaler 4:1
                            // FCY = 40 MHz -> SPI 1.25MHz

    // SPI1CON2 - Not necessary
    // SPI1CON2bits.FRMEN = 1; // Framed SPIx support enabled (SSx pin used as frame sync pulse input/output)
    // SPI1CON2bits.SPIFSD = 0; //  Frame sync pulse output (master)
    // SPI1CON2bits.FRMPOL = 0; // Frame sync pulse is active-low
    // SPI1CON2bits.FRMDLY = 1; // Frame sync pulse coincides with first bit clock

    //SPI1STAT - Not necessary
    // SPI1STATbits.SPISIDL = 0; // continue moduel operation in idle mode
    // SPI1STATbits.SPIROV; // flag de overflow en recibe
    // SPI1STATbits.SPITBF;// flag de full en emite  SPI1TXB
    // SPI1STATbits.SPIRBF; // flag de full en SPI1RXB

    SPI1CON1bits.MSTEN = 1; // Master mode Enabled
    SPI1STATbits.SPIEN = 1; // Enable SPI module
    SPI1BUF = 0x0000;       // Write data to be transmitted

                            // Interrupt Controller Settings
    IFS0bits.SPI1IF = 0;    // Clear the Interrupt Flag
    IEC0bits.SPI1IE = 1;    // Enable the Interrupt
    */
    
}



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


/* Nombre: Refresh
 * Descripción: Funcion que es llamada por la interrupcion cada 0.1ms
                Responsable de la multiplexacion , carga las capas una a una 
                mediante loadLayer 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 
void Refresh(void)
{
    static int ticks = 0;
    static uint8_t layer = 0;
    ticks++;
    if(ticks >= TREFRESH)
    {
        ticks = 0;
        // Se incrementa esta capa
        layer = mod(layer);
        loadLayer(layer);
        layer++;
    }
    

}

/* Nombre: loadLayer
 * Descripción: Funcion de envio de datos en modo bloqueado por SPI
 * Argumentos: el identificador de la capa a enviar al cubo
 * Valor devuelto: Ninguno */ 

void loadLayer(uint8_t layer)
{
    static uint8_t x,y,i,fila;

    // Clear de los shift registers

    _SS = 0;
    //delay_us(TCLEAR);
    for(i = 0 ; i < TCLEAR; i++);

    _SS = 1;

    // Envio del identificador de capa 
    for(i = 7 ; i <= 7 ; i--){
        _SCK = 0;
        if( i == layer)
            _SDO = 1;
        else
            _SDO = 0;
        //delay_us(TSCLK/2);
        Nop();
        Nop();
        _SCK = 1;
        //delay_us(TSCLK/2);
        Nop();
        Nop();
    }


    // Envio de la configuracion de capa
    for(y = 7 ; y <= 7 ; y--)
    {
        fila = voxel[layer][y];
        for(x = 7 ; x <= 7 ; x--)
        {
            _SCK = 0;
            _SDO = fila >> x; //getVoxel(x,y,layer);
            // delay_us(TSCLK/2);
            _SCK = 1;
            // delay_us(TSCLK/2);
            
        }
    }
    _SDO = 0; //
    _SCK = 0;
    //initTimer1();
    
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

/* Nombre: putPlane
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
    uint8_t* config;
    uint8_t y,z;
    coord = mod(coord);
    if(dim == X)
    {   // Caso especial los config son Yrows
        for(z = 0 ; z < N ; z++)
            *config++ = getAxis(Y,coord,z);
    }
    else if(dim == Y)
    {   // Los config son Xrows
        for(z = 0 ; z < N ; z++)
            *config++ = getAxis(X,coord,z);
    }
    else if(dim == Z)
    {   // Los config son Xrows
        for(y = 0 ; y < N ; y++)
            *config++ = getAxis(X,y,coord);
    }

    return config;
}




///////////////////////////////////////////


void drawCube(uint8_t edge,uint8_t x,uint8_t y,uint8_t z)
{
    clearCube();
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


//5 Efecto lluvia
// void rain(void)
// {
//     static int i;
//     i--;
//     if(i<=0)
//         i=7;

//     if(i%2=1){
//         for(j=0;j<=N; j++)
//         {
//             srand (time (NULL));
//             leds=rand()%8;
//             putAxis(X, j, i, leds);
            
//         }
//         putPlane(Z,i-1,getPlane(Z,i));
//     }
// }

//6 Cubo que cambia de tamaño y posicion
        //Cubo tamaño n
        // for(n=8;n<=3;n--){
        //     for(i=0;i<=N;i++){
        //         if(i==n || i==0)
        //         {
        //             for(j=0;j<=N;j++)
        //             {
        //                 if(j==0||j==n)
        //                  putAxis(X,j,i,0xFF>>(n-1));
        //                 else
        //                     putAxis(X,j,i,(0x01||0x01<<(n-1)));
        //             }
        //         }else{
        //             for(j==0||j==n)
        //                 putAxis(X,j,i,(0x01||0x01<<(n-1)));
        //         }
        //     }      
        // }

//7  Efecto anillos que se juntan
// void ringMovement(void)
// {
//     int i,j,z;
//     for(j=-4;j<=11;j++)
//     {
//         z=j;
//         for(i=1;i<=4;i++)
//         {
//             z = j;
//             if(j<=0)
//             {
//                 z=0;
//             }else if(j>=7){
//                 z=7;
//             }
//             ring(i,z);
//             ring(i,7-z);
//         }
//     }
// }

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
/*Nombre: Ring
 * Descripcion: Crea un anillo de lado l
 * Argumentos: l (lado) y z (plano en el que se pone el anillo)
 * Valor devuelto: Ninguno*/
void ring(int l, int z)
{
    int a=4-l;
    putAxis(X, a, z, (0xFF>>2*a)<<a);
    putAxis(X, 7-a, z, (0xFF>>2*a)<<a);
    putAxis(Y, a, z, (0xFF>>2*a)<<a);
    putAxis(Y, 7-a, z, (0xFF>>2*a)<<a);
}



         //Variable d varía con cada ciclo de reloj
void setOblique(int d)
{
    int i,j,k;
    clearCube();
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

void animateCube(int r)
{

    static int x = 0, y = 0, z = 0;
    static int i = 0, j = 0;
    static char growing = true;
    const char min = 3,max = 8;
    if(growing){
        i++;
        if(i > max){
            i = max;
            growing = false;

            j = r;
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
    drawCube(i,x*(8-i),y*(8-i),z*(8-i));    
}

