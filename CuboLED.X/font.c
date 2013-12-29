// Jose Javier Gonzalez Ortiz
// Libreria para el manejo de caracteres alfanumericos en el cubo LED
// font.c


#include "font.h"

void putAscii(uint8_t dim, int8_t coord, char c);
void effect_push_message(char* message,uint8_t dim, uint8_t space,uint8_t reset);
void effect_broadway_message(char* message,uint8_t reset);


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


void putAscii(uint8_t dim, int8_t coord, char c)
{
    putPlane(dim,coord,ascii[c]);
}



void effect_push_message(char* message,uint8_t dim, uint8_t space,uint8_t reset)
{
    static uint8_t i = 0,c=0;
    uint8_t j;

    if(reset)
    {
        i = c = 0;
        putAscii(dim,N-1,message[c]);
    }
    

    shiftCube(dim,true,false);
    i++;
    if(i > space){
        i = 0;
        c++;
        if(message[c] == '\0'){
            c = 0;
        }
        putAscii(dim,N-1,message[c]);
    }

}

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

    if(count >= N-1){
        count = 0;
        letter++;
        if(message[letter-1] == '\0')
            letter = 0;
    }

    buffer[0] = getColumnFont(ascii[message[letter]],N-count++);

    //COPY TO CUBE
    for(i = 0; i < N ; i++)
    {
        putAxis(Z,0,i,buffer[i]);
    }
    for(i = 1; i < N; i++)
    {
        putAxis(Z,i,N-1,buffer[i+7]);
    }
    for(i = 1; i < N; i++)
    {
        putAxis(Z,N-1,N-i-1,buffer[i+14]);
    }
    // for(i = 1; i < N-1; i++)
    // {
    //     putAxis(Z,N-1-i,0,buffer[i+21]);
    // }
}

void effect_slide_message(char* message, uint8_t reset)
{
    static uint8_t buffer[28];
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

    if(count >= N-1){
        count = 0;
        letter++;
        if(message[letter-1] == '\0')
            letter = 0;
    }

    buffer[0] = getColumnFont(ascii[message[letter]],N-count++);

    //COPY TO CUBE
    for(i = 0; i < N ; i++)
    {
        putAxis(Z,3,i,buffer[i]);
        putAxis(Z,4,i,buffer[i]);
    }

}





