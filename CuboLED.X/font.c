// Jose Javier Gonzalez Ortiz
// Libreria para el manejo de caracteres alfanumericos en el cubo LED
// font.c


#include "font.h"

// ----------------------------------- PROTOTIPOS -----------------------------------------

void    putFont                 (uint8_t dim,   uint8_t coord,  uint8_t* character);
void    putAscii                (uint8_t dim,   uint8_t coord,  char c);
uint8_t getColumnFont           (uint8_t* c,    uint8_t column);
void    effect_push_message     (char* message, uint8_t dim,    uint8_t space, uint8_t reset);
void    effect_broadway_message (char* message, uint8_t reset);
void    effect_slide_message    (char* message, uint8_t reset);


// ----------------------------------- FUNCIONES ------------------------------------------

/* Nombre: putFont
 * Descripción: Funcion de activacion de un plano perpendicular al eje (X,Y,Z) del cubo con el caracter correspondiente
 * Argumentos:  dim - dimension del plano 
                    X -> X
                    Y -> Y
                    Z -> Z
                coord1 - coordenada del plano  
                    X -> coordenada en X
                    Y -> coordenada en Y
                    Z -> Coordenanda en Z
                character - mapa bidimensional de constantes que respresentar el caracter
                    
 * Valor devuelto: Ninguno */ 


void putFont(uint8_t dim,  uint8_t coord, uint8_t* character)
{
    uint8_t y,z;
    if(dim == Y || dim == Z){   
        putPlane(dim,coord,character);
    }
    else if(dim == X)
    {
        for(z = 0; z < N; z++)
        {
            for(y = 0; y < N; y++)
            {
                putVoxel(coord,y,z,test(character[z],(N-1)-y));
            }
        }
    }
}



/* Nombre: putAscii
 * Descripción: Funcion de activacion de un plano perpendicular al eje (X,Y,Z) del cubo con el caracter ascii correspondiente
 * Argumentos:  dim - dimension del plano 
                    X -> X
                    Y -> Y
                    Z -> Z
                coord1 - coordenada del plano  
                    X -> coordenada en X
                    Y -> coordenada en Y
                    Z -> Coordenanda en Z
                c - caracter ascii a representar
                    
 * Valor devuelto: Ninguno */ 


void putAscii(uint8_t dim, uint8_t coord, char c)
{
    putFont(dim,coord,ascii[c]);
    // uint8_t y,z;
    // if(dim == Y || dim == Z){   
    //     putPlane(dim,coord,ascii[c]);
    // }
    // else if(dim == X)
    // {
    //     for(z = 0; z < N; z++)
    //     {
    //         for(y = 0; y < N; y++)
    //         {
    //             putVoxel(coord,y,z,test(ascii[c][z],N-1-y));
    //         }
    //     }
    // }
}

/* Nombre: effect_push_message
 * Descripción: Efecto de empujar un mensaje a traves del cubo
 * Argumentos: 
                message - String del mensaje
                dim     - dimension en la que enviar el mensaje (X,Y,Z)
                space   - separacion entre las letras
                reset   - variable de reinicializacion del efecto
 * Valor devuelto: Ninguno */ 

void effect_push_message(char* message,uint8_t dim, uint8_t space,uint8_t reset)
{
    static uint8_t i = 0,c=0;
    uint8_t j;

    if(reset)
    {
        i = c = 0;
        putAscii(dim,N-1,message[c]);
    }
    
    if(dim != Z){
        shiftCube(dim,true,false);
    }else{
        shiftCube(dim,false,false);
    }
    
    i++;
    if(i > space){
        i = 0;
        if(message[c] == '\0'){
            c = 0;
        }
        putAscii(dim,dim != Z ? N-1 : 0,message[c++]);
    }
    

}

/* Nombre: getColumnFont
 * Descripción: Funcion para obtener las columnas de las fuentes (necesario para effect_broadway_message)
 * Argumentos: 
                c       - Fuente del tipo char[8]
                column  - columna a obtener
 * Valor devuelto: config - configuracion de la columna pedida */ 

uint8_t getColumnFont(uint8_t* c, uint8_t column)
{
    uint8_t config = 0,i;
    for(i = 0; i < N; i++)
    {
        if(test(c[i],column)){
            set(config,i);
        }
    }
    return config;
}

/* Nombre: effect_broadway_message
 * Descripción: Efecto de retransimitir por las caras X=0, Y=0 y X= N-1 un mensaje
 * Argumentos: 
                message - String del mensaje
                reset   - variable de reinicializacion del efecto
 * Valor devuelto: Ninguno */ 

void effect_broadway_message(char* message,uint8_t reset)
{
    const uint8_t bufferSize = 22;
    static uint8_t buffer[22];
    static uint8_t count = 0,letter = 0;
    uint8_t i;

    if(reset)
    {   
        count = letter = 0;
        for(i = 0; i < bufferSize; i++)
            buffer[i] = 0x00;
        return;
    }

    for(i = bufferSize-2; i < bufferSize; i--){
        buffer[i+1] = buffer[i];
    }

    if(count >= N){
        count = 0;
        letter++;
        if(message[letter-1] == '\0')
            letter = 0;
    }

    buffer[0] = getColumnFont(ascii[message[letter]],count++);

    //COPY TO CUBE
    // Cara X=0
    for(i = 0; i < N ; i++)
    {
        putAxis(Z,N-1-i,0,buffer[i]);
    }
    // Cara Y=0
    for(i = 1; i < N; i++)
    {
        putAxis(Z,0,i,buffer[i+7]);
    }
    // Cara X = N-1
    for(i = 1; i < N; i++)
    {
        putAxis(Z,i,N-1,buffer[i+14]);
    }
    // Cara Y = N-1
    // for(i = 1; i < N-1; i++)
    // {
    //     putAxis(Z,N-1-i,0,buffer[i+21]);
    // }
}

/* Nombre: effect_slide_message
 * Descripción: Efecto de mostrar de forma lateral un mensaje en los planos Y = 3 e Y = 4
                message - String del mensaje
                reset   - variable de reinicializacion del efecto
 * Valor devuelto: Ninguno */ 

void effect_slide_message(char* message, uint8_t reset)
{
    static uint8_t buffer[8];
    static uint8_t count = 0,letter = 0;
    uint8_t i;

    if(reset)
    {   
        count = letter = 0;
        for(i = 0; i < N; i++)
            buffer[i] = 0x00;
        return;
    }

    for(i = N-2; i < N; i--){
        buffer[i+1] = buffer[i];
    }

    if(count >= N){
        count = 0;
        letter++;
        if(message[letter-1] == '\0')
            letter = 0;
    }

    buffer[0] = getColumnFont(ascii[message[letter]],count++);

    //COPY TO CUBE
    for(i = 0; i < N ; i++)
    {
        putAxis(Z,N-1-i,3,buffer[i]);
        putAxis(Z,N-1-i,4,buffer[i]);
    }

}





