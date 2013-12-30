// Jose Javier Gonzalez Ortiz
// Libreria para el manejo del cubo LED
// cubo.c 

#include "cubo.h"


// ----------------------------------- PROTOTIPOS -----------------------------------------

void        cubeInit        (void);
void        SPI_PinRemap    (void);
void        SPIinit         (void);
void        Refresh         (void);
void        loadLayer       (uint8_t layer);

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



// ----------------------------------- FUNCIONES -----------------------------------------

/* Nombre: cubeInit
 * Descripción: Funcion de incializacion completa del cubo LED
 * Argumentos: Nnguno
 * Valor devuelto: Ninguno */ 

void cubeInit(void)
{
    SPIinit();
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
    RPOR5bits.RP10R = 0x07; // 0b00111;
    //Asign SCK1OUT (Serial clock out) to pin RP7 (RB7)
    RPOR3bits.RP7R  = 0x08; //0b01000;
    // Assign SS1OUT (SPI1 Slave Select) to pin RP6 (RB6)
    // No se usa el SS como tal 
    RPOR3bits.RP6R  = 0x09; // 0b01001;

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

    
    //SPI1CON1
    IFS0bits.SPI1IF = 0;    // Clear the Interrupt Flag
    IEC0bits.SPI1IE = 0;    // Disable the Interrupt

                            // SPI1CON1 Register Settings 
    SPI1CON1bits.DISSCK=0;  // InternalSerialClockisEnabled 
    SPI1CON1bits.DISSDO=0;  // SDO1 pin is controlled by the module
    SPI1CON1bits.MODE16=0;  // Communicationisword-wide(8bits)
    SPI1CON1bits.SMP = 0;   // Serial output data changes on transition
    SPI1CON1bits.CKE = 1; // 0;   // from Idle clock state to active clock state
    SPI1CON1bits.CKP = 0;   // Idle state for clock is a low level;
                            // active state is a high level

    SPI1CON1bits.SPRE = 000;// Secondary prescaler 8:1
    SPI1CON1bits.PPRE = 11; // Primary prescaler 1:1
                            // FCY = 40 MHz -> SPI 5MHz

    SPI1CON1bits.SSEN = 0;  // Se utiza para clear por lo que se deshabilita

    SPI1CON1bits.MSTEN = 1; // Master mode Enabled
    SPI1STATbits.SPIEN = 1; // Enable SPI module

                            // Interrupt Controller Settings
    IFS0bits.SPI1IF = 0;    // Clear the Interrupt Flag
    IEC0bits.SPI1IE = 0;    // Enable the Interrupt
    
    
}

/* Nombre: Refresh
 * Descripción: Funcion que es llamada por la interrupcion cada TREFRESH decimas de ms
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
        layer &= 0x07;
        loadLayer(layer);
        layer++;
        if(layer >= N)
            layer =0;
    }
    

}

/* Nombre: loadLayer
 * Descripción: Funcion de envio de datos por SPI
 * Argumentos: el identificador de la capa a enviar al cubo
 * Valor devuelto: Ninguno */ 

void loadLayer(uint8_t layer)
{
    static uint8_t i;

    // Clear de los shift registers

    _SS = 0;
    //delay_us(TCLEAR);
    for(i = 0 ; i < TCLEAR; i++);
    _SS = 1;
    
    // Envio del identificador de capa 
    
    while(SPI1STATbits.SPITBF);
    SPI1BUF = 0x01 << layer;
    for(i = 7 ; i <= 7 ; i--)
    {
        // Envio de la configuracion de capa
        while(SPI1STATbits.SPITBF);
        SPI1BUF = voxel[layer][i];
    }


    
    
    
}

//////////////////////////////// CUBE FUNCTIONS //////////////////////////////////////////

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
        }else{
            fillPlane(dim,0,0x00);
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
        }else{
            fillPlane(dim,N-1,0x00);
        }
        
    }

}



